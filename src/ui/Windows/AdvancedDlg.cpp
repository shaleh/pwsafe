/*
* Copyright (c) 2003-2010 Rony Shapiro <ronys@users.sourceforge.net>.
* All rights reserved. Use of the code is allowed under the
* Artistic License 2.0 terms, as specified in the LICENSE file
* distributed with this code, or available from
* http://www.opensource.org/licenses/artistic-license-2.0.php
*/
// ADVANCEDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "passwordsafe.h"
#include "ThisMfcApp.h"
#include "GeneralMsgBox.h"
#include "AdvancedDlg.h"

#include "corelib/ItemData.h"
#include "corelib/corelib.h"

#include "resource.h"
#include "resource3.h"

#include <bitset>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CAdvancedDlg::dialog_lookup[ADV_LAST] = {
  IDD_ADVANCED,      // ADV_COMPARE
  IDD_ADVANCEDMERGE, // ADV_MERGE (significantly reduced dialog)
  IDD_ADVANCED,      // ADV_SYNCHRONIZE
  IDD_ADVANCED,      // ADV_EXPORT_TEXT
  IDD_ADVANCED,      // ADV_EXPORT_XML
  IDD_ADVANCED       // ADV_FIND
};

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg dialog

CAdvancedDlg::CAdvancedDlg(CWnd* pParent /* = NULL */, Type iIndex,
                           CItemData::FieldBits bsFields, CString subgroup_name,
                           int subgroup_set, int subgroup_object, int subgroup_function)
  : CPWDialog(dialog_lookup[iIndex], pParent) , m_iIndex(iIndex), 
  m_bsFields(bsFields), m_subgroup_name(subgroup_name), 
  m_subgroup_set(subgroup_set), m_subgroup_object(subgroup_object),
  m_subgroup_function(subgroup_function), m_subgroup_case(BST_UNCHECKED),
  m_pToolTipCtrl(NULL), m_treatwhitespaceasempty(BST_CHECKED)
{
  //{{AFX_DATA_INIT(CAdvancedDlg)
  //}}AFX_DATA_INIT

  if (m_subgroup_function < 0) {
    m_subgroup_case = BST_CHECKED;
    m_subgroup_function *= (-1);
  }

  if (m_subgroup_set == BST_UNCHECKED)
    m_subgroup_name = L"*";

  if (m_bsFields.count() == 0)
    m_bsFields.set();
}

CAdvancedDlg::~CAdvancedDlg()
{
  if (m_iIndex != ADV_MERGE)
    delete m_pToolTipCtrl;
}

BOOL CAdvancedDlg::OnInitDialog()
{
  CPWDialog::OnInitDialog();

  CString cs_text;
  int iItem(-1), i;

  switch (m_iIndex) {
    case ADV_COMPARE:
      cs_text.LoadString(IDS_COMPAREX);
      break;
    case ADV_MERGE:
      cs_text.LoadString(IDS_MERGEX);
      break;
    case ADV_SYNCHRONIZE:
      cs_text.LoadString(IDS_SYNCHRONIZEX);
      break;
    case ADV_EXPORT_TEXT:
      cs_text.LoadString(IDS_EXPORT_TEXTX);
      break;
    case ADV_EXPORT_XML:
      cs_text.LoadString(IDS_EXPORT_XMLX);
      break;
    case ADV_FIND:
      cs_text.LoadString(IDS_FINDX);
      break;
    default:
      ASSERT(FALSE);
  }
  SetWindowText(cs_text);

  CComboBox *cboSubgroupFunction = (CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION);
  if (cboSubgroupFunction->GetCount() == 0) {
    cs_text.LoadString(IDSC_EQUALS);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_EQUALS);
    cs_text.LoadString(IDSC_DOESNOTEQUAL);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_NOTEQUAL);
    cs_text.LoadString(IDSC_BEGINSWITH);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_BEGINS);
    cs_text.LoadString(IDSC_DOESNOTBEGINSWITH);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_NOTBEGIN);
    cs_text.LoadString(IDSC_ENDSWITH);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_ENDS);
    cs_text.LoadString(IDSC_DOESNOTENDWITH);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_NOTEND);
    cs_text.LoadString(IDSC_CONTAINS);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_CONTAINS);
    cs_text.LoadString(IDSC_DOESNOTCONTAIN);
    iItem = cboSubgroupFunction->AddString(cs_text);
    cboSubgroupFunction->SetItemData(iItem, PWSMatch::MR_NOTCONTAIN);
  }

  for (i = 0; i < cboSubgroupFunction->GetCount(); i++) {
    if ((int)cboSubgroupFunction->GetItemData(i) == m_subgroup_function) {
      cboSubgroupFunction->SetCurSel(i);
      break;
    }
  }

  // Note: NOT SORTED by design
  CComboBox *cboSubgroupObject = (CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT);
  if (cboSubgroupObject->GetCount () == 0) {
    cs_text.LoadString(IDS_GROUP);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::GROUP);
    cs_text.LoadString(IDS_GROUPTITLE);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::GROUPTITLE);
    cs_text.LoadString(IDS_TITLE);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::TITLE);
    cs_text.LoadString(IDS_USERNAME);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::USER);
    cs_text.LoadString(IDS_URL);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::URL);
    cs_text.LoadString(IDS_NOTES);
    iItem = cboSubgroupObject->AddString(cs_text);
    cboSubgroupObject->SetItemData(iItem, CItemData::NOTES);
  }

  for (i = 0; i < cboSubgroupObject->GetCount(); i++) {
    if ((int)cboSubgroupObject->GetItemData(i) == m_subgroup_object) {
      cboSubgroupObject->SetCurSel(i);
      break;
    }
  }

  BOOL bEnable = (m_subgroup_set == BST_CHECKED) ? TRUE : FALSE;
  GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_NAME)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_CASE)->EnableWindow(bEnable);

  if (m_iIndex == ADV_MERGE)
    return TRUE;

  // m_pLC_List are those fields that aren't currently selected but could be
  // m_pLC_Selected are those fields already selected
  m_pLC_List = (CListCtrl *)GetDlgItem(IDC_ADVANCED_LIST);
  m_pLC_Selected = (CListCtrl *)GetDlgItem(IDC_ADVANCED_SELECTED);

  m_pLC_List->InsertColumn(0, L"");
  m_pLC_Selected->InsertColumn(0, L"");
  m_pLC_List->SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
  m_pLC_Selected->SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);

  // Deal with non-text fields
  // Time fields - use Compare text and remove the quotes and leading blank
  switch (m_iIndex) {
    case ADV_COMPARE:
      // All these are potential comparison fields
      cs_text.LoadString(IDS_COMPCTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::CTIME, 1));
      cs_text.LoadString(IDS_COMPPMTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::PMTIME, 1));
      cs_text.LoadString(IDS_COMPATIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::ATIME, 1));
      cs_text.LoadString(IDS_COMPRMTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::RMTIME, 1));
      cs_text.LoadString(IDS_COMPXTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::XTIME, 1));
      cs_text.LoadString(IDS_PASSWORDEXPIRYDATEINT);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, MAKELONG(CItemData::XTIME_INT, 1));
      cs_text.LoadString(IDS_PWPOLICY);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, CItemData::POLICY);
      cs_text.LoadString(IDS_DCALONG);
      iItem = m_pLC_List->InsertItem(++iItem, cs_text);
      m_pLC_List->SetItemData(iItem, CItemData::DCA);
      break;
    case ADV_FIND:
      // Don't add any of these - they are not text fields
      break;
    case ADV_MERGE:
    case ADV_SYNCHRONIZE:
    case ADV_EXPORT_TEXT:
    case ADV_EXPORT_XML:
      // All these are already selected fields
      cs_text.LoadString(IDS_COMPCTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::CTIME, 1));
      cs_text.LoadString(IDS_COMPPMTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::PMTIME, 1));
      cs_text.LoadString(IDS_COMPATIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::ATIME, 1));
      cs_text.LoadString(IDS_COMPRMTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::RMTIME, 1));
      cs_text.LoadString(IDS_COMPXTIME);
      cs_text = cs_text.Mid(2, cs_text.GetLength() - 3);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::XTIME, 1));
      cs_text.LoadString(IDS_PASSWORDEXPIRYDATEINT);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, MAKELONG(CItemData::XTIME_INT, 1));
      cs_text.LoadString(IDS_PWPOLICY);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::POLICY);
      cs_text.LoadString(IDS_DCALONG);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::DCA);
      break;
    default:
      ASSERT(FALSE);
      break;
  }

  // Deal with text fields - all selected by default
  cs_text.LoadString(IDS_NOTES);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::NOTES);
  cs_text.LoadString(IDS_URL);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::URL);
  cs_text.LoadString(IDS_AUTOTYPE);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::AUTOTYPE);
  cs_text.LoadString(IDS_PWHISTORY);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::PWHIST);
  cs_text.LoadString(IDS_RUNCOMMAND);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::RUNCMD);
  cs_text.LoadString(IDS_EMAIL);
  iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
  m_pLC_Selected->SetItemData(iItem, CItemData::EMAIL);

  // Deal with standard text fields - selected by default
  switch (m_iIndex) {
    case ADV_EXPORT_XML:
      cs_text.LoadString(IDS_GROUP);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::GROUP);
      cs_text.LoadString(IDS_ADVANCED_TITLETEXT); // <-- Special
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::TITLE);
      cs_text.LoadString(IDS_USERNAME);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::USER);
      cs_text.LoadString(IDS_ADVANCED_PASSWORDTEXT); // <-- Special
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::PASSWORD);
      break;
    case ADV_COMPARE:
      cs_text.LoadString(IDS_ADVANCED_GROUPTEXT); // <-- Special
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::GROUP);
      cs_text.LoadString(IDS_ADVANCED_TITLETEXT); // <-- Special
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::TITLE);
      cs_text.LoadString(IDS_ADVANCED_USERTEXT); // <-- Special
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::USER);
      cs_text.LoadString(IDS_PASSWORD);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::PASSWORD);
      break;
    case ADV_SYNCHRONIZE:
      cs_text.LoadString(IDS_PASSWORD);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::PASSWORD);
      break;
    case ADV_FIND:
    case ADV_MERGE:
    case ADV_EXPORT_TEXT:
      cs_text.LoadString(IDS_GROUP);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::GROUP);
      cs_text.LoadString(IDS_TITLE);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::TITLE);
      cs_text.LoadString(IDS_USERNAME);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::USER);
      cs_text.LoadString(IDS_PASSWORD);
      iItem = m_pLC_Selected->InsertItem(++iItem, cs_text);
      m_pLC_Selected->SetItemData(iItem, CItemData::PASSWORD);
      break;
    default:
      ASSERT(FALSE);
  }

  if (m_bsFields.count() != 0) {
    LVFINDINFO findinfo;
    CString cs_text;
    int iItem;
    DWORD_PTR dw_data;

    SecureZeroMemory(&findinfo, sizeof(findinfo));

    findinfo.flags = LVFI_PARAM;

    for (int i = 0; i < CItemData::LAST; i++) {
      // If selected, leave in the list of selected fields
      if (m_bsFields.test(i))
        continue;

      // Not selected - find entry in list of selected fields and move it
      findinfo.lParam = i;
      iItem = m_pLC_Selected->FindItem(&findinfo);
      if (iItem == -1)
        continue;

      cs_text = m_pLC_Selected->GetItemText(iItem, 0);
      dw_data = m_pLC_Selected->GetItemData(iItem);
      m_pLC_Selected->DeleteItem(iItem);
      iItem = m_pLC_List->InsertItem(0, cs_text);
      m_pLC_List->SetItemData(iItem, dw_data);
    }
  }

  m_pLC_List->SortItems(AdvCompareFunc, NULL);
  m_pLC_Selected->SortItems(AdvCompareFunc, NULL);

  if (m_iIndex != ADV_COMPARE) {
    GetDlgItem(IDC_TREATWHITESPACEASEMPTY)->EnableWindow(FALSE);
    GetDlgItem(IDC_TREATWHITESPACEASEMPTY)->ShowWindow(SW_HIDE);
  } else {
    ((CButton *)GetDlgItem(IDC_TREATWHITESPACEASEMPTY))->SetCheck(BST_CHECKED);
  }

  m_pToolTipCtrl = new CToolTipCtrl;
  if (!m_pToolTipCtrl->Create(this, TTS_ALWAYSTIP | TTS_BALLOON | TTS_NOPREFIX)) {
    TRACE(L"Unable To create Advanced Dialog ToolTip\n");
    delete m_pToolTipCtrl;
    m_pToolTipCtrl = NULL;
    return TRUE;
  }

  // Tooltips
  EnableToolTips();

  // Activate the tooltip control.
  m_pToolTipCtrl->Activate(TRUE);
  m_pToolTipCtrl->SetMaxTipWidth(300);
  // Quadruple the time to allow reading by user
  int iTime = m_pToolTipCtrl->GetDelayTime(TTDT_AUTOPOP);
  m_pToolTipCtrl->SetDelayTime(TTDT_AUTOPOP, 4 * iTime);

  // Set the tooltip
  // Note naming convention: string IDS_xxx corresponds to control IDC_xxx
  CString cs_ToolTip;
  cs_ToolTip.LoadString(IDS_ADVANCED_SELECTSOME);
  m_pToolTipCtrl->AddTool(GetDlgItem(IDC_ADVANCED_SELECTSOME), cs_ToolTip);
  cs_ToolTip.LoadString(IDS_ADVANCED_SELECTALL);
  m_pToolTipCtrl->AddTool(GetDlgItem(IDC_ADVANCED_SELECTALL), cs_ToolTip);
  cs_ToolTip.LoadString(IDS_ADVANCED_DESELECTSOME);
  m_pToolTipCtrl->AddTool(GetDlgItem(IDC_ADVANCED_DESELECTSOME), cs_ToolTip);
  cs_ToolTip.LoadString(IDS_ADVANCED_DESELECTALL);
  m_pToolTipCtrl->AddTool(GetDlgItem(IDC_ADVANCED_DESELECTALL), cs_ToolTip);

  return TRUE;
}

void CAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
  CPWDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAdvancedDlg)
  DDX_Check(pDX, IDC_ADVANCED_SUBGROUP_SET, m_subgroup_set);
  DDX_Check(pDX, IDC_ADVANCED_SUBGROUP_CASE, m_subgroup_case);
  DDX_Text(pDX, IDC_ADVANCED_SUBGROUP_NAME, m_subgroup_name);
  if (m_iIndex == ADV_COMPARE)
    DDX_Check(pDX, IDC_TREATWHITESPACEASEMPTY, m_treatwhitespaceasempty);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdvancedDlg, CPWDialog)
  //{{AFX_MSG_MAP(CAdvancedDlg)
  ON_BN_CLICKED(IDC_ADVANCED_SUBGROUP_SET, OnSetSubGroup)
  ON_BN_CLICKED(IDC_ADVANCED_SELECTSOME, OnSelectSome)
  ON_BN_CLICKED(IDC_ADVANCED_SELECTALL, OnSelectAll)
  ON_BN_CLICKED(IDC_ADVANCED_DESELECTSOME, OnDeselectSome)
  ON_BN_CLICKED(IDC_ADVANCED_DESELECTALL, OnDeselectAll)
  ON_BN_CLICKED(ID_HELP, OnHelp)
  ON_NOTIFY(LVN_ITEMCHANGING, IDC_ADVANCED_SELECTED, OnSelectedItemchanging) 
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedDlg message handlers

void CAdvancedDlg::OnHelp()
{
  CString cs_HelpTopic(app.GetHelpFileName());
  switch (m_iIndex) {
    case ADV_COMPARE:
      cs_HelpTopic += L"::/html/comparex.html";
      break;
    case ADV_MERGE:
      cs_HelpTopic += L"::/html/mergex.html";
      break;
    case ADV_SYNCHRONIZE:
      cs_HelpTopic += L"::/html/synchronizex.html";
      break;
    case ADV_EXPORT_TEXT:
      cs_HelpTopic += L"::/html/exporttextx.html";
      break;
    case ADV_EXPORT_XML:
      cs_HelpTopic += L"::/html/exportxmlx.html";
      break;
    case ADV_FIND:
      cs_HelpTopic += L"::/html/findx.html";
      break;
    default:
      ASSERT(FALSE);
  }
  HtmlHelp(DWORD_PTR((LPCWSTR)cs_HelpTopic), HH_DISPLAY_TOPIC);
}

void CAdvancedDlg::OnOK()
{
  CGeneralMsgBox gmb;
  CString cs_text;
  DWORD_PTR dw_data;

  UpdateData();
  m_bsFields.reset();

  if (m_iIndex != ADV_MERGE) {
    int num_selected = m_pLC_Selected->GetItemCount();
    int nItem(-1);

    for (int i = 0; i < num_selected; i++) {
      nItem = m_pLC_Selected->GetNextItem(nItem, LVNI_ALL);
      dw_data = LOWORD(m_pLC_Selected->GetItemData(nItem));
      m_bsFields.set(dw_data, true);
    }

    if (m_bsFields.count() == 0) {
      CString cs_error_msg;
      switch (m_iIndex) {
        case ADV_COMPARE:
          cs_error_msg.LoadString(IDS_NOFIELDSFORCOMPARE);
          break;
        case ADV_SYNCHRONIZE:
          cs_error_msg.LoadString(IDS_NOFIELDSFORSYNCH);
          break;
        case ADV_EXPORT_TEXT:
          cs_error_msg.LoadString(IDS_NOFIELDSFOREXPORT);
          break;
        case ADV_FIND:
          cs_error_msg.LoadString(IDS_NOFIELDSFORSEARCH);
          break;
        default:
          ASSERT(FALSE);
      }
      gmb.AfxMessageBox(cs_error_msg);
      m_bsFields.set();  // note: impossible to set them all even via the advanced dialog
      return;
    }
  }

  if (m_subgroup_set == BST_CHECKED) {
    GetDlgItemText(IDC_ADVANCED_SUBGROUP_NAME, m_subgroup_name);
    int nObject = ((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT))->GetCurSel();
    if (nObject == CB_ERR) {
      gmb.AfxMessageBox(IDS_NOOBJECT);
      m_bsFields.set();  // note: impossible to set them all even via the advanced dialog
      ((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT))->SetFocus();
      return;
    }

    int nFunction = ((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION))->GetCurSel();
    if (nFunction == CB_ERR) {
      gmb.AfxMessageBox(IDS_NOFUNCTION);
      m_bsFields.set();  // note: impossible to set them all even via the advanced dialog
      ((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION))->SetFocus();
      return;
    }

    m_subgroup_object = int(((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT))->GetItemData(nObject));
    m_subgroup_function = int(((CComboBox *)GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION))->GetItemData(nFunction));
    if (m_subgroup_case == BST_CHECKED)
      m_subgroup_function *= (-1);
  }

  if (m_subgroup_name == L"*")
    m_subgroup_name.Empty();

  CPWDialog::OnOK();
}

void CAdvancedDlg::OnSetSubGroup()
{
  m_subgroup_set = ((CButton*)GetDlgItem(IDC_ADVANCED_SUBGROUP_SET))->GetCheck();

  BOOL bEnable = (m_subgroup_set == BST_CHECKED) ? TRUE : FALSE;
  GetDlgItem(IDC_ADVANCED_SUBGROUP_FUNCTION)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_OBJECT)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_NAME)->EnableWindow(bEnable);
  GetDlgItem(IDC_ADVANCED_SUBGROUP_CASE)->EnableWindow(bEnable);
}

void CAdvancedDlg::OnSelectSome()
{
  CString cs_text;
  int iItem;
  DWORD_PTR dw_data;

  int num_selected = m_pLC_List->GetItemCount();
  if (num_selected == 0)
    return;

  int nItem(-1);

  for (int i = 0; i < num_selected; i++) {
    nItem = m_pLC_List->GetNextItem(nItem, LVNI_SELECTED);
    if (nItem == -1)
      continue;
    cs_text = m_pLC_List->GetItemText(nItem, 0);
    dw_data = m_pLC_List->GetItemData(nItem);
    m_pLC_List->DeleteItem(nItem);
    iItem = m_pLC_Selected->InsertItem(0, cs_text);
    m_pLC_Selected->SetItemData(iItem, dw_data);
  }
  m_pLC_List->SortItems(AdvCompareFunc, NULL);
  m_pLC_Selected->SortItems(AdvCompareFunc, NULL);
}

void CAdvancedDlg::OnSelectAll()
{
  CString cs_text;
  int iItem;
  DWORD_PTR dw_data;

  int num_in_list = m_pLC_List->GetItemCount();
  if (num_in_list == 0)
    return;

  for (int i = 0; i < num_in_list; i++) {
    cs_text = m_pLC_List->GetItemText(i, 0);
    dw_data = m_pLC_List->GetItemData(i);
    iItem = m_pLC_Selected->InsertItem(0, cs_text);
    m_pLC_Selected->SetItemData(iItem, dw_data);
  }
  m_pLC_List->DeleteAllItems();
  m_pLC_Selected->SortItems(AdvCompareFunc, NULL);
}

void CAdvancedDlg::OnDeselectSome()
{
  CString cs_text;
  int iItem;
  DWORD_PTR dw_data;

  int num_selected = m_pLC_Selected->GetItemCount();
  if (num_selected == 0)
    return;

  int nItem(-1);

  for (int i = 0; i < num_selected; i++) {
    nItem = m_pLC_Selected->GetNextItem(nItem, LVNI_SELECTED);
    if (nItem == -1)
      continue;
    cs_text = m_pLC_Selected->GetItemText(nItem, 0);
    dw_data = m_pLC_Selected->GetItemData(nItem);
    m_pLC_Selected->DeleteItem(nItem);
    iItem = m_pLC_List->InsertItem(0, cs_text);
    m_pLC_List->SetItemData(iItem, dw_data);
  }
  m_pLC_List->SortItems(AdvCompareFunc, NULL);
  m_pLC_Selected->SortItems(AdvCompareFunc, NULL);
}

void CAdvancedDlg::OnDeselectAll()
{
  CString cs_text;
  int iItem;
  DWORD_PTR dw_data;

  int num_selected = m_pLC_Selected->GetItemCount();
  if (num_selected == 0)
    return;

  for (int i = num_selected - 1; i >= 0; i--) {
    cs_text = m_pLC_Selected->GetItemText(i, 0);
    dw_data = m_pLC_Selected->GetItemData(i);
    if (m_iIndex == ADV_EXPORT_XML &&
        (dw_data == CItemData::TITLE || dw_data == CItemData::PASSWORD))
      continue;
    if (m_iIndex == ADV_COMPARE &&
        (dw_data == CItemData::GROUP || dw_data == CItemData::TITLE ||
         dw_data == CItemData::USER))
      continue;
    iItem = m_pLC_List->InsertItem(0, cs_text);
    m_pLC_List->SetItemData(iItem, dw_data);
    m_pLC_Selected->DeleteItem(i);
  }
  m_pLC_List->SortItems(AdvCompareFunc, NULL);
}

void CAdvancedDlg::OnSelectedItemchanging(NMHDR * pNMHDR, LRESULT * pResult)
{
  // Prevent TITLE and PASSWORD being deselected in EXPORT_XML function
  // as they are mandatory fields
  // Prevent group, TITLE and USER being deselected in COMPARE function
  // as they are always checked
  NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR; 
  *pResult = FALSE;

  switch (m_iIndex) {
    case ADV_COMPARE:
      if ((pNMListView->lParam == CItemData::GROUP ||
           pNMListView->lParam == CItemData::TITLE ||
           pNMListView->lParam == CItemData::USER) &&
          (pNMListView->uNewState & LVIS_SELECTED)) {
        pNMListView->uNewState &= ~LVIS_SELECTED;
        *pResult = TRUE;
      }
      break;
    case ADV_EXPORT_XML:
      if ((pNMListView->lParam == CItemData::TITLE ||
           pNMListView->lParam == CItemData::PASSWORD) &&
          (pNMListView->uNewState & LVIS_SELECTED)) {
        pNMListView->uNewState &= ~LVIS_SELECTED;
        *pResult = TRUE;
      }
      break;
    case ADV_FIND:
    case ADV_MERGE:
    case ADV_SYNCHRONIZE:
    case ADV_EXPORT_TEXT:
      break;
    default:
      ASSERT(FALSE);
  }
}

// Override PreTranslateMessage() so RelayEvent() can be
// called to pass a mouse message to CPWSOptions's
// tooltip control for processing.
BOOL CAdvancedDlg::PreTranslateMessage(MSG* pMsg)
{
  if (m_pToolTipCtrl != NULL)
    m_pToolTipCtrl->RelayEvent(pMsg);

  return CPWDialog::PreTranslateMessage(pMsg);
}

int CALLBACK CAdvancedDlg::AdvCompareFunc(LPARAM lParam1, LPARAM lParam2,
                                          LPARAM /* closure */)
{
  return (int)(lParam1 - lParam2);
}
