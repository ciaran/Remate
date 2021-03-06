#import <Cocoa/Cocoa.h>

@protocol TMPlugInController
- (float)version;
@end

@interface ReMatePreferencesController : NSWindowController
{
	IBOutlet NSTableView* tableView;
}
- (IBAction)saveAndClose:(id)sender;
@end

@interface ReMate : NSObject
{
	ReMatePreferencesController* configurationWindowController;
	NSMenu* windowMenu;
	id <NSMenuItem> configureMenuItem;
}
- (id)initWithPlugInController:(id <TMPlugInController>)aController;
- (void)dealloc;

- (void)installMenuItem;
- (void)uninstallMenuItem;
+ (id)defaults;
+ (void)setDefaults:(NSArray*)defaults;
@end

// ==================
// = TextMate stuff =
// ==================
@interface OakMenuButton : NSButton
{
@public
    NSMenu* actionMenu;
    NSOutlineView* outlineView;
    id delegate;
    NSPopUpButtonCell* popUpMenuCell;
}

- (void)dealloc;
- (void)awakeFromNib;
- (BOOL)validateMenuItem:(id)fp8;
- (void)mouseDown:(id)fp8;
- (void)popUpMenuDidClose:(id)fp8;
- (void)projectRenameFile:(id)fp8;
@end

@interface NSButton (ProjectContextMenu)
- (void)projectAwakeFromNib;
- (void)refreshProjects:(id)sender;
@end

@interface OakProjectController : NSWindowController
{
@public
    id tabBarView;
    id textView;
    // OakTabBarView *tabBarView;
    // OakTextView *textView;
    id statusBar;
    NSDrawer *groupsAndFilesDrawer;
    NSOutlineView *outlineView;
    NSImageView *imageView;
    NSColor *backgroundColor;
    NSWindow *newFileSheet;
    NSPopUpButton *newFileTemplatePopupButton;
    NSString *newFileSheetFilename;
    NSString *newFileSheetDirectory;
    int newFileCurrentTemplateTag;
    NSString *projectDirectory;
    NSString *filename;
    NSMutableArray *rootItems;
    NSMutableDictionary *currentDocument;
    NSSet *allFilenames;
    NSMutableDictionary *fileMetaData;
    NSMutableArray *projectVariables;
    BOOL showTabBarView;
    BOOL showStatusBarView;
    BOOL canOpenInformationPanel;
    BOOL isScratchProject;
    unsigned int untitledCount;
    BOOL snapshotFrame;
    NSArray *itemsBeingDragged;
    NSMutableDictionary *bindingProxy;
    NSMutableArray *observingItems;
    // OakFileChooser *fileChooser;
    id fileChooser;
    BOOL boldFolders;
}

+ (void)initialize;
+ (BOOL)canOpenFile:(id)fp8;
- (id)init;
- (id)initWithContentsOfFile:(id)fp8;
- (void)prepareToDisposeItems:(id)fp8;
- (void)dealloc;
- (void)applicationWillTerminate:(id)fp8;
- (void)observeValueForKeyPath:(id)fp8 ofObject:(id)fp12 change:(id)fp16 context:(void*)fp20;
- (BOOL)isItemChanged:(id)fp8;
- (void)applicationDidBecomeActiveNotification:(id)fp8;
- (id)textView;
- (id)documentForItem:(id)fp8;
- (id)filename;
- (id)projectDirectory;
- (void)setProjectDirectory:(id)fp8;
- (void)setFilename:(id)fp8;
- (void)setIsScratchProject:(BOOL)fp8;
- (id)displayName;
- (id)windowTitle;
- (void)layoutWindow;
- (void)toggleShowTabBarView:(id)fp8;
- (void)toggleShowStatusBarView:(id)fp8;
- (id)toggleShowTabBarViewMenuTitle;
- (id)toggleShowStatusBarViewMenuTitle;
- (void)openProjectDrawer:(id)fp8;
- (void)windowDidLoad;
- (void)windowDidResize:(id)fp8;
- (void)windowDidMove:(id)fp8;
- (void)setCloseTabActionAsPrimary:(BOOL)fp8;
- (void)tabBarView:(id)fp8 didOpenTab:(id)fp12;
- (void)tabBarView:(id)fp8 didCloseTab:(id)fp12;
- (void)windowDidChangeKey:(id)fp8;
- (void)setCurrentDocument:(id)fp8;
- (void)tabBarView:(id)fp8 didSelectTab:(id)fp12;
- (void)selectItem:(id)fp8;
- (id)itemWithPath:(id)fp8;
- (void)performCloseTab:(id)fp8;
- (void)performCloseAllTabs:(id)fp8;
- (BOOL)tabBarView:(id)fp8 shouldCloseTab:(id)fp12;
- (void)saveProject:(id)fp8;
- (void)saveProjectAs:(id)fp8;
- (void)saveProjectPanelDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(id)fp16;
- (id)mutableDocumentTreeForItems:(id)fp8;
- (id)savableDocumentTreeForItems:(id)fp8 inDirectory:(id)fp12;
- (BOOL)writeToFile:(id)fp8;
- (void)insertItems:(id)fp8 before:(id)fp12;
- (void)insertItemsBeforeSelection:(id)fp8;
- (void)setNewFileCurrentTemplateTag:(int)fp8;
- (void)projectNewFile:(id)fp8;
- (void)newFileSheetPopUpDidChange:(id)fp8;
- (void)performNewFileSheetAction:(id)fp8;
- (void)projectAddFiles:(id)fp8;
- (BOOL)panel:(id)fp8 shouldShowFilename:(id)fp12;
- (void)addFilesPanelDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(void*)fp16;
- (void)projectNewGroup:(id)fp8;
- (void)projectGroupFiles:(id)fp8;
- (BOOL)selectedRowsShouldBeRemovedInOutlineView:(id)fp8;
- (void)projectRemoveFiles:(id)fp8;
- (void)removeProjectFilesWarningDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(void*)fp16;
- (void)userDefaultsDidChange:(id)fp8;
- (int)outlineView:(id)fp8 numberOfChildrenOfItem:(id)fp12;
- (id)outlineView:(id)fp8 child:(int)fp12 ofItem:(id)fp16;
- (BOOL)outlineView:(id)fp8 isItemExpandable:(id)fp12;
- (void)outlineView:(id)fp8 willDisplayCell:(id)fp12 forTableColumn:(id)fp16 item:(id)fp20;
- (id)outlineView:(id)fp8 objectValueForTableColumn:(id)fp12 byItem:(id)fp16;
- (void)outlineView:(id)fp8 setObjectValue:(id)fp12 forTableColumn:(id)fp16 byItem:(id)fp20;
- (void)singleClickItem:(id)fp8;
- (void)doubleClickItem:(id)fp8;
- (BOOL)validateMenuItem:(id)fp8;
- (void)goToNextFile:(id)fp8;
- (void)goToPreviousFile:(id)fp8;
- (void)goToFileCounterpart:(id)fp8;
- (void)revealInProject:(id)fp8;
- (BOOL)respondsToSelector:(SEL)fp8;
- (id)performSelector:(SEL)fp8;
- (id)performSelector:(SEL)fp8 withObject:(id)fp12;
- (id)_getOakTextViewInstance:(id)fp8;
- (BOOL)outlineView:(id)fp8 writeItems:(id)fp12 toPasteboard:(id)fp16;
- (unsigned int)outlineView:(id)fp8 validateDrop:(id)fp12 proposedItem:(id)fp16 proposedChildIndex:(int)fp20;
- (id)addFiles:(id)fp8 toArray:(id)fp12 atIndex:(int)fp16;
- (id)addFiles:(id)fp8 toArray:(id)fp12 atIndex:(int)fp16 fileFilter:(id)fp20 folderFilter:(id)fp24;
- (BOOL)outlineView:(id)fp8 acceptDrop:(id)fp12 item:(id)fp16 childIndex:(int)fp20;
- (void)saveDocument:(id)fp8;
- (void)saveDocumentAs:(id)fp8;
- (void)saveDocumentPanelDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(void*)fp16;
- (BOOL)saveAllDocuments:(id)fp8;
- (void)applicationWillResignActiveNotification:(id)fp8;
- (void)openFileInNewWindow:(id)fp8;
- (void)revealFileInFinder:(id)fp8;
- (void)openFileWithFinder:(id)fp8;
- (void)windowWillClose:(id)fp8;
- (unsigned int)numberOfModifiedDocuments;
- (BOOL)isDocumentEdited;
- (BOOL)windowShouldClose:(id)fp8;
- (void)askToSaveProjectAndCallBack:(id)fp8;
- (void)saveProjectWarningDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(id)fp16;
- (void)askToSaveModifiedDocumentsAndCallBack:(id)fp8;
- (void)saveModifiedDocumentsWarningDidEnd:(id)fp8 returnCode:(int)fp12 contextInfo:(id)fp16;
- (void)askToSaveAndCallBack:(id)fp8;
- (void)closeWindow:(id)fp8 andCallStack:(id)fp12;
- (id)resultForMatchInLine:(id)fp8 fromLine:(unsigned long)fp12 fromColumn:(unsigned long)fp16 toLine:(unsigned long)fp20 toColumn:(unsigned long)fp24 memento:(unsigned long)fp28 fileInfo:(id)fp32;
- (id)findString:(id)fp8 inDocument:(id)fp12 withOptions:(id)fp16;
- (id)findInProjectWithOptions:(id)fp8;
- (void)selectFindInProjectResult:(id)fp8;
- (BOOL)textView:(id)fp8 willExecuteCommand:(id)fp12;
- (void)harvestItemsForArray:(id)fp8 foldersInto:(id)fp12 andFilesInto:(id)fp16;
- (id)findProjectDirectory;
- (id)environmentVariables;
- (void)selectCommandResult:(id)fp8;
- (void)toggleGroupsAndFilesDrawer:(id)fp8;
- (void)outlineViewSelectionDidChange:(id)fp8;
- (void)projectShowInformationPanel:(id)fp8;
- (void)toggleTreatFileAsText:(id)fp8;
- (void)goToFile:(id)fp8;
- (id)openFiles:(id)fp8;

@end
