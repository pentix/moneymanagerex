/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************/


#include "mmOption.h"
#include "import_export/univcsvdialog.h"
#include "constants.h"
#include "images_list.h"
#include "singleton.h"
#include "model/Model_Infotable.h"
#include "model/Model_Setting.h"
#include "model/Model_Account.h"

//----------------------------------------------------------------------------
mmOptions::mmOptions()
:   dateFormat_(mmex::DEFDATEFORMAT)
    , language_("english")
    , databaseUpdated_(false)
{}

//----------------------------------------------------------------------------
mmOptions& mmOptions::instance()
{
    return Singleton<mmOptions>::instance();
}

//----------------------------------------------------------------------------
void mmOptions::LoadInfotableOptions()
{
    dateFormat_ = Model_Infotable::instance().DateFormat();
    userNameString_ = Model_Infotable::instance().GetStringInfo("USERNAME", "");

    financialYearStartDayString_   = Model_Infotable::instance().GetStringInfo("FINANCIAL_YEAR_START_DAY", "1");
    financialYearStartMonthString_ = Model_Infotable::instance().GetStringInfo("FINANCIAL_YEAR_START_MONTH", "7");
}

//----------------------------------------------------------------------------
mmIniOptions::mmIniOptions()
: html_font_size_(100)
, budgetFinancialYears_(false)
, budgetIncludeTransfers_(false)
, budgetSetupWithoutSummaries_(false)
, budgetReportWithSummaries_(true)
, ignoreFutureTransactions_(false)
, transPayeeSelectionNone_(0)
, transCategorySelectionNone_(0)
, transStatusReconciled_(0)
, transDateDefault_(0)

{}

mmIniOptions& mmIniOptions::instance()
{
    return Singleton<mmIniOptions>::instance();
}

void mmIniOptions::loadOptions()
{
    html_font_size_ = Model_Setting::instance().GetHtmlScaleFactor();

    budgetFinancialYears_           = Model_Setting::instance().BudgetFinancialYears();
    budgetIncludeTransfers_         = Model_Setting::instance().BudgetIncludeTransfers();
    budgetSetupWithoutSummaries_    = Model_Setting::instance().BudgetSetupWithoutSummary();
    budgetReportWithSummaries_      = Model_Setting::instance().BudgetSummaryWithoutCategory();
    ignoreFutureTransactions_       = Model_Setting::instance().IgnoreFutureTransactions();

    // Read the preference as a string and convert to int
    transPayeeSelectionNone_ = Model_Setting::instance().GetIntSetting("TRANSACTION_PAYEE_NONE", 0);

    // For the category selection, default behavior should remain that the last category used for the payee is selected.
    //  This is item 1 (0-indexed) in the list.
    transCategorySelectionNone_ = Model_Setting::instance().GetIntSetting("TRANSACTION_CATEGORY_NONE", 1);
    transStatusReconciled_      = Model_Setting::instance().GetIntSetting("TRANSACTION_STATUS_RECONCILED", 0);
    transDateDefault_           = Model_Setting::instance().GetIntSetting("TRANSACTION_DATE_DEFAULT", 0);
}

const int mmIniOptions::account_image_id(int account_id, bool def)
{
    int max = img::MAX_XPM - img::MONEY_DOLLAR_XPM;
    int min = 1;
    int custom_img_id = Model_Infotable::instance().GetIntInfo(wxString::Format("ACC_IMAGE_ID_%i", account_id), 0);
    if (!def && (custom_img_id >= min && custom_img_id <= max))
        return custom_img_id + img::MONEY_DOLLAR_XPM - 1;

    int selectedImage = img::MONEYACCOUNT_XPM; //Default value
    wxString acctStatus = VIEW_ACCOUNTS_OPEN_STR;
    Model_Account::TYPE acctType = Model_Account::CHECKING;
    bool favorite = true;

    Model_Account::Data* account = Model_Account::instance().get(account_id);
    if (account)
    {
        acctType = Model_Account::type(account);
        acctStatus = account->STATUS;
        favorite = account->FAVORITEACCT == "TRUE";
    }
    bool closed = acctStatus == "Closed";

    switch (acctType)
    {
    case (Model_Account::CHECKING) :
        if (closed) selectedImage = img::SAVINGS_ACC_CLOSED_XPM;
        else if (favorite) selectedImage = img::SAVINGS_ACC_FAVORITE_XPM;
        else selectedImage = img::MONEYACCOUNT_XPM;
        break;
    case (Model_Account::TERM) :
        if (closed) selectedImage = img::TERM_ACC_CLOSED_XPM;
        else if (favorite) selectedImage = img::TERM_ACC_FAVORITE_XPM;
        else  selectedImage = img::TERMACCOUNT_XPM;
        break;
    case (Model_Account::INVESTMENT) :
        if (closed) selectedImage = img::STOCK_ACC_CLOSED_XPM;
        else if (favorite) selectedImage = img::STOCK_ACC_FAVORITE_XPM;
        else  selectedImage = img::STOCK_ACC_XPM;
        break;
    case (Model_Account::CREDIT_CARD) :
        if (closed) selectedImage = img::CARD_ACC_CLOSED_XPM;
        else if (favorite)   selectedImage = img::CARD_ACC_FAVORITE_XPM;
        else   selectedImage = img::CARD_ACC_XPM;
        break;
    default:
        wxASSERT(false);
    }
    return selectedImage;
}

