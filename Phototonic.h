/*
 *  Copyright (C) 2013-2015 Ofer Kashayov <oferkv@live.com>
 *  This file is part of Phototonic Image Viewer.
 *
 *  Phototonic is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Phototonic is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Phototonic.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "ColorsDialog.h"
#include "CopyMoveToDialog.h"
#include "CropDialog.h"
#include "MetadataCache.h"
#include "ResizeDialog.h"

#include <QActionGroup>
#include <QMainWindow>
#include <QPointer>
#include <QStackedLayout>
#include <QTreeWidgetItem>

#define VERSION "Phototonic v2.1"

class BookMarks;
class ImageViewer;
class ThumbsViewer;
class FileListWidget;
class FileSystemTree;

class Phototonic : public QMainWindow {
    Q_OBJECT

public:
    int copyCutThumbsCount;

    Phototonic(const QStringList &argumentsList, int filesStartAt, QWidget *parent = nullptr);

    QMenu *createPopupMenu();

    void setStatus(const QString &state);

    void showBusyAnimation(bool busy);

    QIcon &getDefaultWindowIcon() { return defaultApplicationIcon; }

    enum CentralWidgets
    {
        ThumbViewWidget = 0,
        ImageViewWidget
    };

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

public slots:

    bool event(QEvent *event);

    void dropOp(Qt::KeyboardModifiers keyMods, bool dirOp, QString copyMoveDirPath);

    void showViewer();

    void loadSelectedThumbImage(const QModelIndex &idx);

    void loadImageFromCliArguments(const QString &cliFileName);

    void hideViewer();

    void setSaveDirectory(const QString &path = QString());

private slots:

    void about();

    void sortThumbnails();

    void reload();

    void setIncludeSubDirs();

    void showSettings();

    void toggleFullScreen();

    void updateActions();

    void onReloadThumbs();

    void findDuplicateImages();

    void renameDir();

    void setThumbsViewerWindowTitle();

    void rename();

    void removeMetadata();

    void viewImage();

    void newImage();

    void addNewBookmark();

    void deleteDirectory(bool trash);

    void createSubDirectory();

    void checkDirState(const QModelIndex &, int, int);

    void goSelectedDir(const QModelIndex &currDir);

    void bookmarkClicked(QTreeWidgetItem *item, int col);

    void goPathBarDir();

    void setThumbsFilter();

    void clearThumbsFilter();

    void goBack();

    void goTo(const QString &path);

    void goForward();

    void goUp();

    void goHome();

    void toggleSlideShow();

    void slideShowHandler();

    void loadNextImage();

    void loadPreviousImage();

    void loadFirstImage();

    void loadLastImage();

    void loadRandomImage();

    void updateIndexByViewerImage();

    void selectAllThumbs();

    void selectByBrightness();

    void deleteOperation();

    void deletePermanentlyOperation();

    void cutThumbs();

    void copyThumbs();

    void pasteThumbs();

    void thumbsZoomIn();

    void thumbsZoomOut();

    void zoomIn(float multiplier = 1.);

    void zoomOut(float multiplier = 1.);

    void resetZoom();

    void origZoom();

    void keepZoom();

    void keepTransformClicked();

    void rotateLeft();

    void rotateRight();

    void flipVertical();

    void cropImage();

    void scaleImage();

    void freeRotateLeft();

    void freeRotateRight();

    void batchTransform();

    void showColorsDialog();

    void setMirrorDisabled();

    void setMirrorDual();

    void setMirrorTriple();

    void setMirrorVDual();

    void setMirrorQuad();

    void flipHorizontal();

    void addVerticalGuide();

    void addHorizontalGuide();

    void moveRight();

    void moveLeft();

    void moveUp();

    void moveDown();

    void setDocksVisibility(bool visible);

    void goTop();

    void goBottom();

    void showHiddenFiles();

    void toggleImageViewerToolbar();

    void setToolbarIconSize();

    void chooseExternalApp();

    void updateExternalApps();

    void runExternalApp();

    void externalAppError();

    void setEditToolBarVisibility();

    void setGoToolBarVisibility();

    void setViewToolBarVisibility();

    void setImageToolBarVisibility();

    void setFileSystemDockVisibility();

    void setBookmarksDockVisibility();

    void setImagePreviewDockVisibility();

    void setTagsDockVisibility();

    void setImageInfoDockVisibility();

    void lockDocks();

    void cleanupCropDialog();

    void cleanupResizeDialog();

    void cleanupColorsDialog();

    void filterImagesFocus();

    void setPathFocus();

    void copyImagesTo();

    void moveImagesTo();

    void onFileListSelected();

    void setSquareThumbs();

    void setCompactThumbs();

    void setClassicThumbs();

    void toggleRotateEnabled();

    void onToolsUpdated();

private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *goMenu;
    QMenu *sortMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *zoomSubMenu;
    QMenu *transformSubMenu;
    QMenu *batchSubMenu;
    QMenu *viewSubMenu;
    QMenu *MirroringSubMenu;
    QMenu *guideSubMenu;
    QMenu *openWithSubMenu;

    QToolBar *viewToolBar;
    QToolBar *editToolBar;
    QToolBar *goToolBar;
    QToolBar *imageToolBar;

    QAction *exitAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *copyToAction;
    QAction *moveToAction;
    QAction *deleteAction;
    QAction *deletePermanentlyAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *renameAction;
    QAction *removeMetadataAction;
    QAction *selectAllAction;
    QAction *selectByBrightnesAction;
    QAction *copyImageAction;
    QAction *pasteImageAction;
    QAction *showClipboardAction;
    QAction *addBookmarkAction;
    QAction *removeBookmarkAction;

    QActionGroup *thumbLayoutsGroup;
    QAction *setClassicThumbsAction;
    QAction *setSquareThumbsAction;
    QAction *setCompactThumbsAction;

    QActionGroup *sortTypesGroup;
    QAction *sortByNameAction;
    QAction *sortByTimeAction;
    QAction *sortBySizeAction;
    QAction *sortByTypeAction;
    QAction *sortBySimilarityAction;
    QAction *sortReverseAction;
    QAction *refreshAction;
    QAction *includeSubDirectoriesAction;
    QAction *fullScreenAction;
    QAction *thumbsGoToTopAction;
    QAction *thumbsGoToBottomAction;
    QAction *CloseImageAction;
    QAction *settingsAction;
    QAction *thumbsZoomInAction;
    QAction *thumbsZoomOutAction;
    QAction *zoomSubMenuAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *resetZoomAction;
    QAction *origZoomAction;
    QAction *keepZoomAction;
    QAction *keepTransformAction;
    QAction *transformSubMenuAction;
    QAction *batchSubMenuAction;
    QAction *viewSubMenuAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *rotateToolAction;
    QAction *flipHorizontalAction;
    QAction *flipVerticalAction;
    QAction *cropAction;
    QAction *applyCropAndRotationAction;
    QAction *resizeAction;
    QAction *freeRotateLeftAction;
    QAction *freeRotateRightAction;
    QAction *colorsAction;
    QActionGroup *mirroringActionGroup;
    QAction *mirrorSubMenuAction;
    QAction *mirrorDisabledAction;
    QAction *mirrorDualAction;
    QAction *mirrorTripleAction;
    QAction *mirrorDualVerticalAction;
    QAction *mirrorQuadAction;
    QAction *guideSubMenuAction;
    QAction *guideAddVerticalAction;
    QAction *guideAddHorizontalAction;
    QAction *moveLeftAction;
    QAction *moveRightAction;
    QAction *moveUpAction;
    QAction *moveDownAction;

    QAction *aboutAction;
    QAction *showHiddenFilesAction;
    QAction *smallToolbarIconsAction;
    QAction *lockDocksAction;
    QAction *showViewerToolbarAction;

    QAction *pasteAction;
    QAction *createDirectoryAction;
    QAction *setSaveDirectoryAction;

    QAction *goBackAction;
    QAction *goFrwdAction;
    QAction *goUpAction;
    QAction *goHomeAction;

    QAction *slideShowAction;
    QAction *nextImageAction;
    QAction *prevImageAction;
    QAction *firstImageAction;
    QAction *lastImageAction;
    QAction *randomImageAction;
    QAction *viewImageAction;
    QAction *filterImagesFocusAction;
    QAction *setPathFocusAction;
    QAction *findDupesAction;

    QAction *openWithMenuAction;
    QAction *externalAppsAction;
    QAction *invertSelectionAction;
    QAction *batchTransformAction;

    QLineEdit *pathLineEdit;
    QLineEdit *filterLineEdit;
    QLabel *statusLabel;
    QDockWidget *fileSystemDock;
    QDockWidget *bookmarksDock;
    QDockWidget *imagePreviewDock;
    QDockWidget *tagsDock;
    FileSystemTree *fileSystemTree;
    BookMarks *bookmarks;
    QDockWidget *imageInfoDock;
    ThumbsViewer *thumbsViewer;
    ImageViewer *imageViewer;
    QList<QString> pathHistoryList;
    QTimer *SlideShowTimer;
    QPointer<CopyMoveToDialog> copyMoveToDialog;
    QWidget *fileSystemDockOrigWidget;
    QWidget *bookmarksDockOrigWidget;
    QWidget *imagePreviewDockOrigWidget;
    QWidget *tagsDockOrigWidget;
    QWidget *imageInfoDockOrigWidget;
    QWidget *fileSystemDockEmptyWidget;
    QWidget *bookmarksDockEmptyWidget;
    QWidget *imagePreviewDockEmptyWidget;
    QWidget *tagsDockEmptyWidget;
    QWidget *imageInfoDockEmptyWidget;
    bool interfaceDisabled;
    std::shared_ptr<MetadataCache> metadataCache;
    FileListWidget *fileListWidget;
    QStackedLayout *stackedLayout;

    int currentHistoryIdx;
    bool needHistoryRecord;
    bool initComplete;
    bool needThumbsRefresh;
    bool shouldMaximize;

    bool editToolBarVisible;
    bool goToolBarVisible;
    bool viewToolBarVisible;
    bool imageToolBarVisible;

    QMovie *busyMovie;
    QLabel *busyLabel;
    QPointer<ResizeDialog> resizeDialog;
    QPointer<ColorsDialog> colorsDialog;
    QPointer<CropDialog> cropDialog;
    QIcon defaultApplicationIcon;

    void refreshThumbs(bool noScroll);

    void loadShortcuts();

    void setupDocks();

    void deleteImages(bool trash);

    void deleteFromViewer(bool trash);

    void loadCurrentImage(int currentRow);

    void selectCurrentViewDir();

    void processStartupArguments(const QStringList &argumentsList, int filesStartAt);

    void loadStartupFileList(const QStringList &argumentsList, int filesStartAt);

    void addMenuSeparator(QWidget *widget);

    void createImageViewer();

    void createThumbsViewer();

    void createActions();

    void createMenus();

    void createToolBars();

    void createStatusBar();

    void createFileSystemDock();

    void createBookmarksDock();

    void createImagePreviewDock();

    void createImageTagsDock();

    void writeSettings();

    void readSettings();

    void addPathHistoryRecord(const QString &dir);

    bool isValidPath(const QString &path);

    [[nodiscard]] QString getSelectedPath() const;

    void setCopyCutActions(bool setEnabled);

    void wheelEvent(QWheelEvent *event);

    void copyOrCutThumbs(bool copy);

    void showNewImageWarning();

    bool removeDirectoryOperation(const QString &dirToDelete);

    void setInterfaceEnabled(bool enable);

    void addBookmark(const QString &path);

    void copyOrMoveImages(bool move);

    void setViewerKeyEventsEnabled(bool enabled);
};
