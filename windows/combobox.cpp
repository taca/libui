// 20 may 2015
#include "uipriv_windows.hpp"

// TODO
// - is there extra space on the bottom?

// we as Common Controls 6 users don't need to worry about the height of comboboxes; see http://blogs.msdn.com/b/oldnewthing/archive/2006/03/10/548537.aspx

struct uiCombobox {
	uiWindowsControl c;
	HWND hwnd;
	void (*onSelected)(uiCombobox *, void *);
	void *onSelectedData;
};

// TODO: NOT triggered on entering text
static BOOL onWM_COMMAND(uiControl *cc, HWND hwnd, WORD code, LRESULT *lResult)
{
	uiCombobox *c = uiCombobox(cc);

	if (code != CBN_SELCHANGE)
		return FALSE;
	(*(c->onSelected))(c, c->onSelectedData);
	*lResult = 0;
	return TRUE;
}

void uiComboboxDestroy(uiControl *cc)
{
	uiCombobox *c = uiCombobox(cc);

	uiWindowsUnregisterWM_COMMANDHandler(c->hwnd);
	uiWindowsEnsureDestroyWindow(c->hwnd);
	uiFreeControl(uiControl(c));
}

uiWindowsControlAllDefaultsExceptDestroy(uiCombobox)

// from http://msdn.microsoft.com/en-us/library/windows/desktop/dn742486.aspx#sizingandspacing
#define comboboxWidth 107 /* this is actually the shorter progress bar width, but Microsoft only indicates as wide as necessary; TODO */
#define comboboxHeight 14

static void uiComboboxMinimumSize(uiWindowsControl *cc, intmax_t *width, intmax_t *height)
{
	uiCombobox *c = uiCombobox(cc);
	uiWindowsSizing sizing;
	int x, y;

	x = comboboxWidth;
	y = comboboxHeight;
	uiWindowsGetSizing(c->hwnd, &sizing);
	uiWindowsSizingDlgUnitsToPixels(&sizing, &x, &y);
	*width = x;
	*height = y;
}

static void defaultOnSelected(uiCombobox *c, void *data)
{
	// do nothing
}

void uiComboboxAppend(uiCombobox *c, const char *text)
{
	WCHAR *wtext;
	LRESULT res;

	wtext = toUTF16(text);
	res = SendMessageW(c->hwnd, CB_ADDSTRING, 0, (LPARAM) wtext);
	if (res == (LRESULT) CB_ERR)
		logLastError(L"error appending item to uiCombobox");
	else if (res == (LRESULT) CB_ERRSPACE)
		logLastError(L"memory exhausted appending item to uiCombobox");
	uiFree(wtext);
}

intmax_t uiComboboxSelected(uiCombobox *c)
{
	LRESULT n;

	n = SendMessage(c->hwnd, CB_GETCURSEL, 0, 0);
	if (n == (LRESULT) CB_ERR)
		return -1;
	return (intmax_t) n;
}

void uiComboboxSetSelected(uiCombobox *c, intmax_t n)
{
	// TODO error check
	SendMessageW(c->hwnd, CB_SETCURSEL, (WPARAM) n, 0);
}

void uiComboboxOnSelected(uiCombobox *c, void (*f)(uiCombobox *c, void *data), void *data)
{
	c->onSelected = f;
	c->onSelectedData = data;
}

static uiCombobox *finishNewCombobox(DWORD style)
{
	uiCombobox *c;

	uiWindowsNewControl(uiCombobox, c);

	c->hwnd = uiWindowsEnsureCreateControlHWND(WS_EX_CLIENTEDGE,
		L"combobox", L"",
		style | WS_TABSTOP,
		hInstance, NULL,
		TRUE);

	uiWindowsRegisterWM_COMMANDHandler(c->hwnd, onWM_COMMAND, uiControl(c));
	uiComboboxOnSelected(c, defaultOnSelected, NULL);

	return c;
}

uiCombobox *uiNewCombobox(void)
{
	return finishNewCombobox(CBS_DROPDOWNLIST);
}

uiCombobox *uiNewEditableCombobox(void)
{
	return finishNewCombobox(CBS_DROPDOWN);
}