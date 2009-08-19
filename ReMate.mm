#import "ReMate.h"
#import "MethodSwizzle.h"

static NSMenuItem* refreshMenuItem;

#define DEFAULTS_KEY @"ReMate Volumes"

@implementation NSWindowController (AppSwitching)
- (NSString*)volumeName
{
	// For new projects
	if([[self valueForKey:@"rootItems"] count] == 0)
		return nil;

	NSString* rootPath = [[[self valueForKey:@"rootItems"] objectAtIndex:0] objectForKey:@"sourceDirectory"];
	NSString* volumeName = nil;

	if([rootPath hasPrefix:@"/Volumes/"])
		volumeName = [[rootPath componentsSeparatedByString:@"/"] objectAtIndex:2];

	return volumeName;
}

- (NSMutableDictionary*)configurationForVolume:(NSString*)volumeName
{
	id volumeConfiguration = nil;
	unsigned int volumeCount = [[ReMate defaults] count];

	for(unsigned int index = 0; index < volumeCount; index += 1)
	{
		NSDictionary* volume = [[ReMate defaults] objectAtIndex:index];
		
		if([[volume objectForKey:@"name"] isEqualToString:volumeName])
		{
			volumeConfiguration = volume;
			break;
		}
	}
	return volumeConfiguration;
}

- (void)ReMate_windowDidLoad
{
	[self ReMate_windowDidLoad];
	
	NSString* volumeName = [self volumeName];

	if(volumeName && [self configurationForVolume:volumeName] == nil)
	{
		NSArray* currentDefaults = [ReMate defaults];
		// Disable refreshing for new volumes by default
		NSDictionary* newVolume = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:YES],@"refreshIsDisabled",
																			volumeName,@"name",
																			nil];
		[ReMate setDefaults:[currentDefaults arrayByAddingObject:newVolume]];
	}
}

- (void)swizzledApplicationDidBecomeActiveNotification:(id)sender
{
	BOOL refreshingIsEnabled = YES;

	if(NSString* volumeName = [self volumeName])
		refreshingIsEnabled = ! [[[self configurationForVolume:volumeName] objectForKey:@"refreshIsDisabled"] boolValue];

	if(refreshingIsEnabled)
		[self swizzledApplicationDidBecomeActiveNotification:sender];
}
@end

@implementation NSButton (ProjectContextMenu)
- (void)projectAwakeFromNib
{
	[self projectAwakeFromNib];
	
	if (![[self valueForKey:@"delegate"] isKindOfClass:NSClassFromString(@"OakProjectController")])
		return;
	
	NSMenu* menu = [self valueForKey:@"actionMenu"];
	[menu addItem:[NSMenuItem separatorItem]];

	refreshMenuItem = [menu addItemWithTitle:@"Refresh Project Files" action:@selector(refreshProjects:) keyEquivalent:@""];
	[refreshMenuItem setTarget:self];
}

- (void)refreshProjects:(id)sender
{
	[((OakMenuButton*)self)->delegate swizzledApplicationDidBecomeActiveNotification:sender];
}

- (BOOL)ReMate_validateMenuItem:(id <NSMenuItem>)menuItem
{
	return ([menuItem action] == @selector(refreshProjects:)) ? YES : [self ReMate_validateMenuItem:menuItem];
}
@end

@implementation ReMate
static id ReMateDefaults = nil;

- (float)version
{
	return 0.1;
}

+ (id)defaults
{
	if (!ReMateDefaults)
		ReMateDefaults = [NSUserDefaults standardUserDefaults];
	return [ReMateDefaults arrayForKey:DEFAULTS_KEY];
}

+ (void)setDefaults:(NSArray*)defaults
{
	[ReMateDefaults setObject:defaults forKey:DEFAULTS_KEY];
}

- (id)initWithPlugInController:(id <TMPlugInController>)aController
{
	if (![ReMate defaults])
		[ReMate setDefaults:[NSArray array]];

	MethodSwizzle(NSClassFromString(@"OakProjectController"), @selector(applicationDidBecomeActiveNotification:), @selector(swizzledApplicationDidBecomeActiveNotification:));
	MethodSwizzle(NSClassFromString(@"OakProjectController"), @selector(windowDidLoad), @selector(ReMate_windowDidLoad));

	MethodSwizzle(NSClassFromString(@"OakMenuButton"), @selector(awakeFromNib), @selector(projectAwakeFromNib));
	MethodSwizzle(NSClassFromString(@"OakMenuButton"), @selector(validateMenuItem:), @selector(ReMate_validateMenuItem:));

	NSApp = [NSApplication sharedApplication];

	if(self = [super init])
		[self installMenuItem];

	return self;
}

- (void)dealloc
{
	[self uninstallMenuItem];
	[super dealloc];
}

- (void)installMenuItem
{
	if (windowMenu = [[[[NSApp mainMenu] itemWithTitle:@"Window"] submenu] retain]) {
		unsigned index = 0;
		NSArray* items = [windowMenu itemArray];
		for (int separators = 0; index != [items count] && separators != 2; index++)
			separators += [[items objectAtIndex:index] isSeparatorItem] ? 1 : 0;

		configureMenuItem = [[NSMenuItem alloc] initWithTitle:@"Configure ReMate" action:@selector(orderFrontConfigureDialog:) keyEquivalent:@""];
		[configureMenuItem setTarget:self];
		[configureMenuItem setState:NSOffState];
		[windowMenu insertItem:configureMenuItem atIndex:index ? index-1 : 0];
		[configureMenuItem release];
	}
}

- (void)orderFrontConfigureDialog:(id)sender
{
	if (!configurationWindowController) {
		ReMatePreferencesController* obj = [ReMatePreferencesController alloc]; // this is a little hacky, since initXYZ could change the obj, but the path variant of initWithNib needs an owner
		NSString* nibPath = [[NSBundle bundleForClass:[self class]] pathForResource:@"Configure" ofType:@"nib"];
		configurationWindowController = [obj initWithWindowNibPath:nibPath owner:obj];
	}

	[configurationWindowController showWindow:self];
}

- (void)uninstallMenuItem
{
	[windowMenu removeItem:configureMenuItem];

	[configureMenuItem release];
	configureMenuItem = nil;

	[windowMenu release];
	windowMenu = nil;
}
@end

@implementation ReMatePreferencesController
- (int)numberOfRowsInTableView:(NSTableView*)aTableView
{
	return [[ReMate defaults] count];
}

- (id)tableView:(NSTableView*)aTableView objectValueForTableColumn:(NSTableColumn*)column row:(int)rowIndex
{
	NSDictionary* config = [[ReMate defaults] objectAtIndex:rowIndex];
	id value = nil;

	if ([[column identifier] isEqualToString:@"name"])
		value = [config objectForKey:@"name"];
	else if ([[column identifier] isEqualToString:@"refreshingEnabled"])
		value = [NSNumber numberWithBool:! [[config objectForKey:@"refreshIsDisabled"] boolValue]];

	return value;
}

- (void)tableView:(NSTableView*)aTableView setObjectValue:(id)value forTableColumn:(NSTableColumn*)column row:(int)rowIndex
{
	NSMutableArray* defaults = [[[ReMate defaults] mutableCopy] autorelease];
	NSMutableDictionary* config = [[[defaults objectAtIndex:rowIndex] mutableCopy] autorelease];
	[config setObject:[NSNumber numberWithBool:![value boolValue]] forKey:@"refreshIsDisabled"];
	[defaults replaceObjectAtIndex:rowIndex withObject:config];
	[ReMate setDefaults:defaults];
}

- (IBAction)saveAndClose:(id)sender
{
	[[sender window] performClose:self];
}

- (IBAction)showWindow:(id)sender
{
	[super showWindow:sender];
	[tableView reloadData];
}
@end
