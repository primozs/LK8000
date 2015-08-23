/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id: dlgOracle.cpp,v 1.1 2011/12/21 10:29:29 root Exp root $
*/

#include "externs.h"
#include "Dialogs.h"
#include "TraceThread.h"
#include "WindowControls.h"
#include "dlgTools.h"
#include "Sound/Sound.h"
#include "Topology.h"

static WndForm *wf=NULL;

extern void ResetNearestTopology();

static void OnCloseClicked(WndButton* pWnd){
    (void)pWnd;
	wf->SetModalResult(mrOK);
}

static CallBackTableEntry_t CallBackTable[]={
  ClickNotifyCallbackEntry(OnCloseClicked),
  EndCallBackEntry()
};

short WaitToCallForce=0;

//#define DEBUG_ORTIMER

WhereAmI _WhereAmI;

// Remember that this function is called at 10hz
static bool OnTimerNotify() {

  if(!_WhereAmI.IsDone()) {
    return false;
  }

  wf->SetTimerNotify(0, NULL);

  // Bell, and print results
  LKSound(TEXT("LK_GREEN.WAV"));
  wf->SetBackColor(RGB_WINBACKGROUND);

  WindowControl* pWndClose = wf->FindByName(_T("cmdClose"));
  if(pWndClose) {
    pWndClose->SetVisible(true);
    pWndClose->SetFocus();
  }
  WndFrame* pWndText = static_cast<WndFrame*>(wf->FindByName(_T("WndText")));
  if (pWndText) {
    pWndText->SetBackColor(RGB_WINBACKGROUND);
    pWndText->SetCaption(_WhereAmI.getText());
  }

  return true;
}


void dlgOracleShowModal(void){

  SHOWTHREAD(_T("dlgOracleShowModal"));

  wf=NULL;
 
  if (!ScreenLandscape) {
    TCHAR filename[MAX_PATH];
    LocalPathS(filename, TEXT("dlgOracle_L.xml"));
    wf = dlgLoadFromXML(CallBackTable, filename, TEXT("IDR_XML_ORACLE_L"));
  } else  {
    TCHAR filename[MAX_PATH];
    LocalPathS(filename, TEXT("dlgOracle.xml"));
    wf = dlgLoadFromXML(CallBackTable, filename, TEXT("IDR_XML_ORACLE"));
  }

  if (!wf) return;
  _WhereAmI.Start();

  WindowControl* pWndClose = wf->FindByName(_T("cmdClose"));
  if (pWndClose) {
    pWndClose->SetVisible(false);
  }
  WndFrame* pWndText = static_cast<WndFrame*>(wf->FindByName(_T("WndText")));
  if (pWndText) {
    pWndText->SetFont(MapWindowBoldFont);
    pWndText->SetCaptionStyle(DT_EXPANDTABS | DT_CENTER | DT_NOCLIP | DT_WORDBREAK);
    TCHAR szText[200] = {};
    _stprintf(szText, _T("\n\n%s\n\n%s"), gettext(_T("_@M1691_")), gettext(_T("_@M1692_")));
    pWndText->SetCaption(szText);
  }

  // We must wait for data ready, so we shall do it  with timer notify.
  wf->SetTimerNotify(100, OnTimerNotify);
  wf->ShowModal();

  delete wf;
  wf = NULL;
}
