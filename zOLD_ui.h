// TODO add a uiVerifyControlType() function that can be used by control implementations to verify controls

// TODOs
// - make getters that return whether something exists accept a NULL pointer to discard the value (and thus only return that the thing exists?)
// - const-correct everything
// - normalize documentation between typedefs and structs

// This constant is provided because M_PI is nonstandard.
// This comes from Go's math.Pi, which in turn comes from http://oeis.org/A000796.
#define uiPi 3.14159265358979323846264338327950288419716939937510582097494459

// TODO uiBool?

// uiForEach represents the return value from one of libui's various ForEach functions.
uiprivEnum(uiForEach) {
	uiForEachContinue,
	uiForEachStop,
};

typedef struct uiInitOptions uiInitOptions;

struct uiInitOptions {
	size_t Size;
};

uiprivExtern void uiFreeInitError(const char *err);

uiprivExtern void uiMain(void);
uiprivExtern void uiMainSteps(void);
uiprivExtern int uiMainStep(int wait);
uiprivExtern void uiQuit(void);

uiprivExtern void uiQueueMain(void (*f)(void *data), void *data);

// TODO standardize the looping behavior return type, either with some enum or something, and the test expressions throughout the code
// TODO figure out what to do about looping and the exact point that the timer is rescheduled so we can document it; see https://github.com/andlabs/libui/pull/277
// TODO (also in the above link) document that this cannot be called from any thread, unlike uiQueueMain()
// TODO document that the minimum exact timing, either accuracy (timer burst, etc.) or granularity (15ms on Windows, etc.), is OS-defined
// TODO also figure out how long until the initial tick is registered on all platforms to document
// TODO also add a comment about how useful this could be in bindings, depending on the language being bound to
uiprivExtern void uiTimer(int milliseconds, int (*f)(void *data), void *data);

uiprivExtern void uiOnShouldQuit(int (*f)(void *data), void *data);

uiprivExtern void uiFreeText(char *text);

typedef struct uiControl uiControl;

struct uiControl {
	uint32_t Signature;
	uint32_t OSSignature;
	uint32_t TypeSignature;
	void (*Destroy)(uiControl *);
	uintptr_t (*Handle)(uiControl *);
	uiControl *(*Parent)(uiControl *);
	void (*SetParent)(uiControl *, uiControl *);
	int (*Toplevel)(uiControl *);
	int (*Visible)(uiControl *);
	void (*Show)(uiControl *);
	void (*Hide)(uiControl *);
	int (*Enabled)(uiControl *);
	void (*Enable)(uiControl *);
	void (*Disable)(uiControl *);
};
// TOOD add argument names to all arguments
#define uiControl(this) ((uiControl *) (this))
uiprivExtern void uiControlDestroy(uiControl *);
uiprivExtern uintptr_t uiControlHandle(uiControl *);
uiprivExtern uiControl *uiControlParent(uiControl *);
uiprivExtern void uiControlSetParent(uiControl *, uiControl *);
uiprivExtern int uiControlToplevel(uiControl *);
uiprivExtern int uiControlVisible(uiControl *);
uiprivExtern void uiControlShow(uiControl *);
uiprivExtern void uiControlHide(uiControl *);
uiprivExtern int uiControlEnabled(uiControl *);
uiprivExtern void uiControlEnable(uiControl *);
uiprivExtern void uiControlDisable(uiControl *);

uiprivExtern uiControl *uiAllocControl(size_t n, uint32_t OSsig, uint32_t typesig, const char *typenamestr);
uiprivExtern void uiFreeControl(uiControl *);

// TODO make sure all controls have these
uiprivExtern void uiControlVerifySetParent(uiControl *, uiControl *);
uiprivExtern int uiControlEnabledToUser(uiControl *);

uiprivExtern void uiUserBugCannotSetParentOnToplevel(const char *type);

typedef struct uiWindow uiWindow;
#define uiWindow(this) ((uiWindow *) (this))
uiprivExtern char *uiWindowTitle(uiWindow *w);
uiprivExtern void uiWindowSetTitle(uiWindow *w, const char *title);
uiprivExtern void uiWindowContentSize(uiWindow *w, int *width, int *height);
uiprivExtern void uiWindowSetContentSize(uiWindow *w, int width, int height);
uiprivExtern int uiWindowFullscreen(uiWindow *w);
uiprivExtern void uiWindowSetFullscreen(uiWindow *w, int fullscreen);
uiprivExtern void uiWindowOnContentSizeChanged(uiWindow *w, void (*f)(uiWindow *, void *), void *data);
uiprivExtern void uiWindowOnClosing(uiWindow *w, int (*f)(uiWindow *w, void *data), void *data);
uiprivExtern int uiWindowBorderless(uiWindow *w);
uiprivExtern void uiWindowSetBorderless(uiWindow *w, int borderless);
uiprivExtern void uiWindowSetChild(uiWindow *w, uiControl *child);
uiprivExtern int uiWindowMargined(uiWindow *w);
uiprivExtern void uiWindowSetMargined(uiWindow *w, int margined);
uiprivExtern uiWindow *uiNewWindow(const char *title, int width, int height, int hasMenubar);

typedef struct uiButton uiButton;
#define uiButton(this) ((uiButton *) (this))
uiprivExtern char *uiButtonText(uiButton *b);
uiprivExtern void uiButtonSetText(uiButton *b, const char *text);
uiprivExtern void uiButtonOnClicked(uiButton *b, void (*f)(uiButton *b, void *data), void *data);
uiprivExtern uiButton *uiNewButton(const char *text);

typedef struct uiBox uiBox;
#define uiBox(this) ((uiBox *) (this))
uiprivExtern void uiBoxAppend(uiBox *b, uiControl *child, int stretchy);
uiprivExtern void uiBoxDelete(uiBox *b, int index);
uiprivExtern int uiBoxPadded(uiBox *b);
uiprivExtern void uiBoxSetPadded(uiBox *b, int padded);
uiprivExtern uiBox *uiNewHorizontalBox(void);
uiprivExtern uiBox *uiNewVerticalBox(void);

typedef struct uiCheckbox uiCheckbox;
#define uiCheckbox(this) ((uiCheckbox *) (this))
uiprivExtern char *uiCheckboxText(uiCheckbox *c);
uiprivExtern void uiCheckboxSetText(uiCheckbox *c, const char *text);
uiprivExtern void uiCheckboxOnToggled(uiCheckbox *c, void (*f)(uiCheckbox *c, void *data), void *data);
uiprivExtern int uiCheckboxChecked(uiCheckbox *c);
uiprivExtern void uiCheckboxSetChecked(uiCheckbox *c, int checked);
uiprivExtern uiCheckbox *uiNewCheckbox(const char *text);

typedef struct uiEntry uiEntry;
#define uiEntry(this) ((uiEntry *) (this))
uiprivExtern char *uiEntryText(uiEntry *e);
uiprivExtern void uiEntrySetText(uiEntry *e, const char *text);
uiprivExtern void uiEntryOnChanged(uiEntry *e, void (*f)(uiEntry *e, void *data), void *data);
uiprivExtern int uiEntryReadOnly(uiEntry *e);
uiprivExtern void uiEntrySetReadOnly(uiEntry *e, int readonly);
uiprivExtern uiEntry *uiNewEntry(void);
uiprivExtern uiEntry *uiNewPasswordEntry(void);
uiprivExtern uiEntry *uiNewSearchEntry(void);

typedef struct uiLabel uiLabel;
#define uiLabel(this) ((uiLabel *) (this))
uiprivExtern char *uiLabelText(uiLabel *l);
uiprivExtern void uiLabelSetText(uiLabel *l, const char *text);
uiprivExtern uiLabel *uiNewLabel(const char *text);

typedef struct uiTab uiTab;
#define uiTab(this) ((uiTab *) (this))
uiprivExtern void uiTabAppend(uiTab *t, const char *name, uiControl *c);
uiprivExtern void uiTabInsertAt(uiTab *t, const char *name, int before, uiControl *c);
uiprivExtern void uiTabDelete(uiTab *t, int index);
uiprivExtern int uiTabNumPages(uiTab *t);
uiprivExtern int uiTabMargined(uiTab *t, int page);
uiprivExtern void uiTabSetMargined(uiTab *t, int page, int margined);
uiprivExtern uiTab *uiNewTab(void);

typedef struct uiGroup uiGroup;
#define uiGroup(this) ((uiGroup *) (this))
uiprivExtern char *uiGroupTitle(uiGroup *g);
uiprivExtern void uiGroupSetTitle(uiGroup *g, const char *title);
uiprivExtern void uiGroupSetChild(uiGroup *g, uiControl *c);
uiprivExtern int uiGroupMargined(uiGroup *g);
uiprivExtern void uiGroupSetMargined(uiGroup *g, int margined);
uiprivExtern uiGroup *uiNewGroup(const char *title);

// spinbox/slider rules:
// setting value outside of range will automatically clamp
// initial value is minimum
// complaint if min >= max?

typedef struct uiSpinbox uiSpinbox;
#define uiSpinbox(this) ((uiSpinbox *) (this))
uiprivExtern int uiSpinboxValue(uiSpinbox *s);
uiprivExtern void uiSpinboxSetValue(uiSpinbox *s, int value);
uiprivExtern void uiSpinboxOnChanged(uiSpinbox *s, void (*f)(uiSpinbox *s, void *data), void *data);
uiprivExtern uiSpinbox *uiNewSpinbox(int min, int max);

typedef struct uiSlider uiSlider;
#define uiSlider(this) ((uiSlider *) (this))
uiprivExtern int uiSliderValue(uiSlider *s);
uiprivExtern void uiSliderSetValue(uiSlider *s, int value);
uiprivExtern void uiSliderOnChanged(uiSlider *s, void (*f)(uiSlider *s, void *data), void *data);
uiprivExtern uiSlider *uiNewSlider(int min, int max);

typedef struct uiProgressBar uiProgressBar;
#define uiProgressBar(this) ((uiProgressBar *) (this))
uiprivExtern int uiProgressBarValue(uiProgressBar *p);
uiprivExtern void uiProgressBarSetValue(uiProgressBar *p, int n);
uiprivExtern uiProgressBar *uiNewProgressBar(void);

typedef struct uiSeparator uiSeparator;
#define uiSeparator(this) ((uiSeparator *) (this))
uiprivExtern uiSeparator *uiNewHorizontalSeparator(void);
uiprivExtern uiSeparator *uiNewVerticalSeparator(void);

typedef struct uiCombobox uiCombobox;
#define uiCombobox(this) ((uiCombobox *) (this))
uiprivExtern void uiComboboxAppend(uiCombobox *c, const char *text);
uiprivExtern int uiComboboxSelected(uiCombobox *c);
uiprivExtern void uiComboboxSetSelected(uiCombobox *c, int n);
uiprivExtern void uiComboboxOnSelected(uiCombobox *c, void (*f)(uiCombobox *c, void *data), void *data);
uiprivExtern uiCombobox *uiNewCombobox(void);

typedef struct uiEditableCombobox uiEditableCombobox;
#define uiEditableCombobox(this) ((uiEditableCombobox *) (this))
uiprivExtern void uiEditableComboboxAppend(uiEditableCombobox *c, const char *text);
uiprivExtern char *uiEditableComboboxText(uiEditableCombobox *c);
uiprivExtern void uiEditableComboboxSetText(uiEditableCombobox *c, const char *text);
// TODO what do we call a function that sets the currently selected item and fills the text field with it? editable comboboxes have no consistent concept of selected item
uiprivExtern void uiEditableComboboxOnChanged(uiEditableCombobox *c, void (*f)(uiEditableCombobox *c, void *data), void *data);
uiprivExtern uiEditableCombobox *uiNewEditableCombobox(void);

typedef struct uiRadioButtons uiRadioButtons;
#define uiRadioButtons(this) ((uiRadioButtons *) (this))
uiprivExtern void uiRadioButtonsAppend(uiRadioButtons *r, const char *text);
uiprivExtern int uiRadioButtonsSelected(uiRadioButtons *r);
uiprivExtern void uiRadioButtonsSetSelected(uiRadioButtons *r, int n);
uiprivExtern void uiRadioButtonsOnSelected(uiRadioButtons *r, void (*f)(uiRadioButtons *, void *), void *data);
uiprivExtern uiRadioButtons *uiNewRadioButtons(void);

struct tm;
typedef struct uiDateTimePicker uiDateTimePicker;
#define uiDateTimePicker(this) ((uiDateTimePicker *) (this))
// TODO document that tm_wday and tm_yday are undefined, and tm_isdst should be -1
// TODO document that for both sides
// TODO document time zone conversions or lack thereof
// TODO for Time: define what values are returned when a part is missing
uiprivExtern void uiDateTimePickerTime(uiDateTimePicker *d, struct tm *time);
uiprivExtern void uiDateTimePickerSetTime(uiDateTimePicker *d, const struct tm *time);
uiprivExtern void uiDateTimePickerOnChanged(uiDateTimePicker *d, void (*f)(uiDateTimePicker *, void *), void *data);
uiprivExtern uiDateTimePicker *uiNewDateTimePicker(void);
uiprivExtern uiDateTimePicker *uiNewDatePicker(void);
uiprivExtern uiDateTimePicker *uiNewTimePicker(void);

// TODO provide a facility for entering tab stops?
typedef struct uiMultilineEntry uiMultilineEntry;
#define uiMultilineEntry(this) ((uiMultilineEntry *) (this))
uiprivExtern char *uiMultilineEntryText(uiMultilineEntry *e);
uiprivExtern void uiMultilineEntrySetText(uiMultilineEntry *e, const char *text);
uiprivExtern void uiMultilineEntryAppend(uiMultilineEntry *e, const char *text);
uiprivExtern void uiMultilineEntryOnChanged(uiMultilineEntry *e, void (*f)(uiMultilineEntry *e, void *data), void *data);
uiprivExtern int uiMultilineEntryReadOnly(uiMultilineEntry *e);
uiprivExtern void uiMultilineEntrySetReadOnly(uiMultilineEntry *e, int readonly);
uiprivExtern uiMultilineEntry *uiNewMultilineEntry(void);
uiprivExtern uiMultilineEntry *uiNewNonWrappingMultilineEntry(void);

typedef struct uiArea uiArea;
typedef struct uiAreaHandler uiAreaHandler;
typedef struct uiAreaDrawParams uiAreaDrawParams;
typedef struct uiAreaMouseEvent uiAreaMouseEvent;
typedef struct uiAreaKeyEvent uiAreaKeyEvent;

typedef struct uiDrawContext uiDrawContext;

struct uiAreaHandler {
	void (*Draw)(uiAreaHandler *, uiArea *, uiAreaDrawParams *);
	// TODO document that resizes cause a full redraw for non-scrolling areas; implementation-defined for scrolling areas
	void (*MouseEvent)(uiAreaHandler *, uiArea *, uiAreaMouseEvent *);
	// TODO document that on first show if the mouse is already in the uiArea then one gets sent with left=0
	// TODO what about when the area is hidden and then shown again?
	void (*MouseCrossed)(uiAreaHandler *, uiArea *, int left);
	void (*DragBroken)(uiAreaHandler *, uiArea *);
	int (*KeyEvent)(uiAreaHandler *, uiArea *, uiAreaKeyEvent *);
};

// TODO RTL layouts?
// TODO reconcile edge and corner naming
uiprivEnum(uiWindowResizeEdge) {
	uiWindowResizeEdgeLeft,
	uiWindowResizeEdgeTop,
	uiWindowResizeEdgeRight,
	uiWindowResizeEdgeBottom,
	uiWindowResizeEdgeTopLeft,
	uiWindowResizeEdgeTopRight,
	uiWindowResizeEdgeBottomLeft,
	uiWindowResizeEdgeBottomRight,
	// TODO have one for keyboard resizes?
	// TODO GDK doesn't seem to have any others, including for keyboards...
	// TODO way to bring up the system menu instead?
};

#define uiArea(this) ((uiArea *) (this))
// TODO give a better name
// TODO document the types of width and height
uiprivExtern void uiAreaSetSize(uiArea *a, int width, int height);
// TODO uiAreaQueueRedraw()
uiprivExtern void uiAreaQueueRedrawAll(uiArea *a);
uiprivExtern void uiAreaScrollTo(uiArea *a, double x, double y, double width, double height);
// TODO document these can only be called within Mouse() handlers
// TODO should these be allowed on scrolling areas?
// TODO decide which mouse events should be accepted; Down is the only one guaranteed to work right now
// TODO what happens to events after calling this up to and including the next mouse up?
// TODO release capture?
uiprivExtern void uiAreaBeginUserWindowMove(uiArea *a);
uiprivExtern void uiAreaBeginUserWindowResize(uiArea *a, uiWindowResizeEdge edge);
uiprivExtern uiArea *uiNewArea(uiAreaHandler *ah);
uiprivExtern uiArea *uiNewScrollingArea(uiAreaHandler *ah, int width, int height);

struct uiAreaDrawParams {
	uiDrawContext *Context;

	// TODO document that this is only defined for nonscrolling areas
	double AreaWidth;
	double AreaHeight;

	double ClipX;
	double ClipY;
	double ClipWidth;
	double ClipHeight;
};

typedef struct uiDrawPath uiDrawPath;
typedef struct uiDrawBrush uiDrawBrush;
typedef struct uiDrawStrokeParams uiDrawStrokeParams;
typedef struct uiDrawMatrix uiDrawMatrix;

typedef struct uiDrawBrushGradientStop uiDrawBrushGradientStop;

uiprivEnum(uiDrawBrushType) {
	uiDrawBrushTypeSolid,
	uiDrawBrushTypeLinearGradient,
	uiDrawBrushTypeRadialGradient,
	uiDrawBrushTypeImage,
};

uiprivEnum(uiDrawLineCap) {
	uiDrawLineCapFlat,
	uiDrawLineCapRound,
	uiDrawLineCapSquare,
};

uiprivEnum(uiDrawLineJoin) {
	uiDrawLineJoinMiter,
	uiDrawLineJoinRound,
	uiDrawLineJoinBevel,
};

// this is the default for botoh cairo and Direct2D (in the latter case, from the C++ helper functions)
// Core Graphics doesn't explicitly specify a default, but NSBezierPath allows you to choose one, and this is the initial value
// so we're good to use it too!
#define uiDrawDefaultMiterLimit 10.0

uiprivEnum(uiDrawFillMode) {
	uiDrawFillModeWinding,
	uiDrawFillModeAlternate,
};

struct uiDrawMatrix {
	double M11;
	double M12;
	double M21;
	double M22;
	double M31;
	double M32;
};

struct uiDrawBrush {
	uiDrawBrushType Type;

	// solid brushes
	double R;
	double G;
	double B;
	double A;

	// gradient brushes
	double X0;		// linear: start X, radial: start X
	double Y0;		// linear: start Y, radial: start Y
	double X1;		// linear: end X, radial: outer circle center X
	double Y1;		// linear: end Y, radial: outer circle center Y
	double OuterRadius;		// radial gradients only
	uiDrawBrushGradientStop *Stops;
	size_t NumStops;
	// TODO extend mode
	// cairo: none, repeat, reflect, pad; no individual control
	// Direct2D: repeat, reflect, pad; no individual control
	// Core Graphics: none, pad; before and after individually
	// TODO cairo documentation is inconsistent about pad

	// TODO images

	// TODO transforms
};

struct uiDrawBrushGradientStop {
	double Pos;
	double R;
	double G;
	double B;
	double A;
};

struct uiDrawStrokeParams {
	uiDrawLineCap Cap;
	uiDrawLineJoin Join;
	// TODO what if this is 0? on windows there will be a crash with dashing
	double Thickness;
	double MiterLimit;
	double *Dashes;
	// TOOD what if this is 1 on Direct2D?
	// TODO what if a dash is 0 on Cairo or Quartz?
	size_t NumDashes;
	double DashPhase;
};

uiprivExtern uiDrawPath *uiDrawNewPath(uiDrawFillMode fillMode);
uiprivExtern void uiDrawFreePath(uiDrawPath *p);

uiprivExtern void uiDrawPathNewFigure(uiDrawPath *p, double x, double y);
uiprivExtern void uiDrawPathNewFigureWithArc(uiDrawPath *p, double xCenter, double yCenter, double radius, double startAngle, double sweep, int negative);
uiprivExtern void uiDrawPathLineTo(uiDrawPath *p, double x, double y);
// notes: angles are both relative to 0 and go counterclockwise
// TODO is the initial line segment on cairo and OS X a proper join?
// TODO what if sweep < 0?
uiprivExtern void uiDrawPathArcTo(uiDrawPath *p, double xCenter, double yCenter, double radius, double startAngle, double sweep, int negative);
uiprivExtern void uiDrawPathBezierTo(uiDrawPath *p, double c1x, double c1y, double c2x, double c2y, double endX, double endY);
// TODO quadratic bezier
uiprivExtern void uiDrawPathCloseFigure(uiDrawPath *p);

// TODO effect of these when a figure is already started
uiprivExtern void uiDrawPathAddRectangle(uiDrawPath *p, double x, double y, double width, double height);

uiprivExtern void uiDrawPathEnd(uiDrawPath *p);

uiprivExtern void uiDrawStroke(uiDrawContext *c, uiDrawPath *path, uiDrawBrush *b, uiDrawStrokeParams *p);
uiprivExtern void uiDrawFill(uiDrawContext *c, uiDrawPath *path, uiDrawBrush *b);

// TODO primitives:
// - rounded rectangles
// - elliptical arcs
// - quadratic bezier curves

uiprivExtern void uiDrawMatrixSetIdentity(uiDrawMatrix *m);
uiprivExtern void uiDrawMatrixTranslate(uiDrawMatrix *m, double x, double y);
uiprivExtern void uiDrawMatrixScale(uiDrawMatrix *m, double xCenter, double yCenter, double x, double y);
uiprivExtern void uiDrawMatrixRotate(uiDrawMatrix *m, double x, double y, double amount);
uiprivExtern void uiDrawMatrixSkew(uiDrawMatrix *m, double x, double y, double xamount, double yamount);
uiprivExtern void uiDrawMatrixMultiply(uiDrawMatrix *dest, uiDrawMatrix *src);
uiprivExtern int uiDrawMatrixInvertible(uiDrawMatrix *m);
uiprivExtern int uiDrawMatrixInvert(uiDrawMatrix *m);
uiprivExtern void uiDrawMatrixTransformPoint(uiDrawMatrix *m, double *x, double *y);
uiprivExtern void uiDrawMatrixTransformSize(uiDrawMatrix *m, double *x, double *y);

uiprivExtern void uiDrawTransform(uiDrawContext *c, uiDrawMatrix *m);

// TODO add a uiDrawPathStrokeToFill() or something like that
uiprivExtern void uiDrawClip(uiDrawContext *c, uiDrawPath *path);

uiprivExtern void uiDrawSave(uiDrawContext *c);
uiprivExtern void uiDrawRestore(uiDrawContext *c);

// uiAttribute stores information about an attribute in a
// uiAttributedString.
//
// You do not create uiAttributes directly; instead, you create a
// uiAttribute of a given type using the specialized constructor
// functions. For every Unicode codepoint in the uiAttributedString,
// at most one value of each attribute type can be applied.
//
// uiAttributes are immutable and the uiAttributedString takes
// ownership of the uiAttribute object once assigned, copying its
// contents as necessary.
typedef struct uiAttribute uiAttribute;

// @role uiAttribute destructor
// uiFreeAttribute() frees a uiAttribute. You generally do not need to
// call this yourself, as uiAttributedString does this for you. In fact,
// it is an error to call this function on a uiAttribute that has been
// given to a uiAttributedString. You can call this, however, if you
// created a uiAttribute that you aren't going to use later.
uiprivExtern void uiFreeAttribute(uiAttribute *a);

// uiAttributeType holds the possible uiAttribute types that may be
// returned by uiAttributeGetType(). Refer to the documentation for
// each type's constructor function for details on each type.
uiprivEnum(uiAttributeType) {
	uiAttributeTypeFamily,
	uiAttributeTypeSize,
	uiAttributeTypeWeight,
	uiAttributeTypeItalic,
	uiAttributeTypeStretch,
	uiAttributeTypeColor,
	uiAttributeTypeBackground,
	uiAttributeTypeUnderline,
	uiAttributeTypeUnderlineColor,
	uiAttributeTypeFeatures,
};

// uiAttributeGetType() returns the type of a.
// TODO I don't like this name
uiprivExtern uiAttributeType uiAttributeGetType(const uiAttribute *a);

// uiNewFamilyAttribute() creates a new uiAttribute that changes the
// font family of the text it is applied to. family is copied; you do not
// need to keep it alive after uiNewFamilyAttribute() returns. Font
// family names are case-insensitive.
uiprivExtern uiAttribute *uiNewFamilyAttribute(const char *family);

// uiAttributeFamily() returns the font family stored in a. The
// returned string is owned by a. It is an error to call this on a
// uiAttribute that does not hold a font family.
uiprivExtern const char *uiAttributeFamily(const uiAttribute *a);

// uiNewSizeAttribute() creates a new uiAttribute that changes the
// size of the text it is applied to, in typographical points.
uiprivExtern uiAttribute *uiNewSizeAttribute(double size);

// uiAttributeSize() returns the font size stored in a. It is an error to
// call this on a uiAttribute that does not hold a font size.
uiprivExtern double uiAttributeSize(const uiAttribute *a);

// uiTextWeight represents possible text weights. These roughly
// map to the OS/2 text weight field of TrueType and OpenType
// fonts, or to CSS weight numbers. The named constants are
// nominal values; the actual values may vary by font and by OS,
// though this isn't particularly likely. Any value between
// uiTextWeightMinimum and uiTextWeightMaximum, inclusive,
// is allowed.
//
// Note that due to restrictions in early versions of Windows, some
// fonts have "special" weights be exposed in many programs as
// separate font families. This is perhaps most notable with
// Arial Black. libui does not do this, even on Windows (because the
// DirectWrite API libui uses on Windows does not do this); to
// specify Arial Black, use family Arial and weight uiTextWeightBlack.
uiprivEnum(uiTextWeight) {
	uiTextWeightMinimum = 0,
	uiTextWeightThin = 100,
	uiTextWeightUltraLight = 200,
	uiTextWeightLight = 300,
	uiTextWeightBook = 350,
	uiTextWeightNormal = 400,
	uiTextWeightMedium = 500,
	uiTextWeightSemiBold = 600,
	uiTextWeightBold = 700,
	uiTextWeightUltraBold = 800,
	uiTextWeightHeavy = 900,
	uiTextWeightUltraHeavy = 950,
	uiTextWeightMaximum = 1000,
};

// uiNewWeightAttribute() creates a new uiAttribute that changes the
// weight of the text it is applied to. It is an error to specify a weight
// outside the range [uiTextWeightMinimum,
// uiTextWeightMaximum].
uiprivExtern uiAttribute *uiNewWeightAttribute(uiTextWeight weight);

// uiAttributeWeight() returns the font weight stored in a. It is an error
// to call this on a uiAttribute that does not hold a font weight.
uiprivExtern uiTextWeight uiAttributeWeight(const uiAttribute *a);

// uiTextItalic represents possible italic modes for a font. Italic
// represents "true" italics where the slanted glyphs have custom
// shapes, whereas oblique represents italics that are merely slanted
// versions of the normal glyphs. Most fonts usually have one or the
// other.
uiprivEnum(uiTextItalic) {
	uiTextItalicNormal,
	uiTextItalicOblique,
	uiTextItalicItalic,
};

// uiNewItalicAttribute() creates a new uiAttribute that changes the
// italic mode of the text it is applied to. It is an error to specify an
// italic mode not specified in uiTextItalic.
uiprivExtern uiAttribute *uiNewItalicAttribute(uiTextItalic italic);

// uiAttributeItalic() returns the font italic mode stored in a. It is an
// error to call this on a uiAttribute that does not hold a font italic
// mode.
uiprivExtern uiTextItalic uiAttributeItalic(const uiAttribute *a);

// uiTextStretch represents possible stretches (also called "widths")
// of a font.
//
// Note that due to restrictions in early versions of Windows, some
// fonts have "special" stretches be exposed in many programs as
// separate font families. This is perhaps most notable with
// Arial Condensed. libui does not do this, even on Windows (because
// the DirectWrite API libui uses on Windows does not do this); to
// specify Arial Condensed, use family Arial and stretch
// uiTextStretchCondensed.
uiprivEnum(uiTextStretch) {
	uiTextStretchUltraCondensed,
	uiTextStretchExtraCondensed,
	uiTextStretchCondensed,
	uiTextStretchSemiCondensed,
	uiTextStretchNormal,
	uiTextStretchSemiExpanded,
	uiTextStretchExpanded,
	uiTextStretchExtraExpanded,
	uiTextStretchUltraExpanded,
};

// uiNewStretchAttribute() creates a new uiAttribute that changes the
// stretch of the text it is applied to. It is an error to specify a strech
// not specified in uiTextStretch.
uiprivExtern uiAttribute *uiNewStretchAttribute(uiTextStretch stretch);

// uiAttributeStretch() returns the font stretch stored in a. It is an
// error to call this on a uiAttribute that does not hold a font stretch.
uiprivExtern uiTextStretch uiAttributeStretch(const uiAttribute *a);

// uiNewColorAttribute() creates a new uiAttribute that changes the
// color of the text it is applied to. It is an error to specify an invalid
// color.
uiprivExtern uiAttribute *uiNewColorAttribute(double r, double g, double b, double a);

// uiAttributeColor() returns the text color stored in a. It is an
// error to call this on a uiAttribute that does not hold a text color.
uiprivExtern void uiAttributeColor(const uiAttribute *a, double *r, double *g, double *b, double *alpha);

// uiNewBackgroundAttribute() creates a new uiAttribute that
// changes the background color of the text it is applied to. It is an
// error to specify an invalid color.
uiprivExtern uiAttribute *uiNewBackgroundAttribute(double r, double g, double b, double a);

// TODO reuse uiAttributeColor() for background colors, or make a new function...

// uiUnderline specifies a type of underline to use on text.
uiprivEnum(uiUnderline) {
	uiUnderlineNone,
	uiUnderlineSingle,
	uiUnderlineDouble,
	uiUnderlineSuggestion,		// wavy or dotted underlines used for spelling/grammar checkers
};

// uiNewUnderlineAttribute() creates a new uiAttribute that changes
// the type of underline on the text it is applied to. It is an error to
// specify an underline type not specified in uiUnderline.
uiprivExtern uiAttribute *uiNewUnderlineAttribute(uiUnderline u);

// uiAttributeUnderline() returns the underline type stored in a. It is
// an error to call this on a uiAttribute that does not hold an underline
// style.
uiprivExtern uiUnderline uiAttributeUnderline(const uiAttribute *a);

// uiUnderlineColor specifies the color of any underline on the text it
// is applied to, regardless of the type of underline. In addition to
// being able to specify a custom color, you can explicitly specify
// platform-specific colors for suggestion underlines; to use them
// correctly, pair them with uiUnderlineSuggestion (though they can
// be used on other types of underline as well).
// 
// If an underline type is applied but no underline color is
// specified, the text color is used instead. If an underline color
// is specified without an underline type, the underline color
// attribute is ignored, but not removed from the uiAttributedString.
uiprivEnum(uiUnderlineColor) {
	uiUnderlineColorCustom,
	uiUnderlineColorSpelling,
	uiUnderlineColorGrammar,
	uiUnderlineColorAuxiliary,		// for instance, the color used by smart replacements on macOS or in Microsoft Office
};

// uiNewUnderlineColorAttribute() creates a new uiAttribute that
// changes the color of the underline on the text it is applied to.
// It is an error to specify an underline color not specified in
// uiUnderlineColor.
//
// If the specified color type is uiUnderlineColorCustom, it is an
// error to specify an invalid color value. Otherwise, the color values
// are ignored and should be specified as zero.
uiprivExtern uiAttribute *uiNewUnderlineColorAttribute(uiUnderlineColor u, double r, double g, double b, double a);

// uiAttributeUnderlineColor() returns the underline color stored in
// a. It is an error to call this on a uiAttribute that does not hold an
// underline color.
uiprivExtern void uiAttributeUnderlineColor(const uiAttribute *a, uiUnderlineColor *u, double *r, double *g, double *b, double *alpha);

// uiOpenTypeFeatures represents a set of OpenType feature
// tag-value pairs, for applying OpenType features to text.
// OpenType feature tags are four-character codes defined by
// OpenType that cover things from design features like small
// caps and swashes to language-specific glyph shapes and
// beyond. Each tag may only appear once in any given
// uiOpenTypeFeatures instance. Each value is a 32-bit integer,
// often used as a Boolean flag, but sometimes as an index to choose
// a glyph shape to use.
// 
// If a font does not support a certain feature, that feature will be
// ignored. (TODO verify this on all OSs)
// 
// See the OpenType specification at
// https://www.microsoft.com/typography/otspec/featuretags.htm
// for the complete list of available features, information on specific
// features, and how to use them.
// TODO invalid features
typedef struct uiOpenTypeFeatures uiOpenTypeFeatures;

// uiOpenTypeFeaturesForEachFunc is the type of the function
// invoked by uiOpenTypeFeaturesForEach() for every OpenType
// feature in otf. Refer to that function's documentation for more
// details.
typedef uiForEach (*uiOpenTypeFeaturesForEachFunc)(const uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t value, void *data);

// @role uiOpenTypeFeatures constructor
// uiNewOpenTypeFeatures() returns a new uiOpenTypeFeatures
// instance, with no tags yet added.
uiprivExtern uiOpenTypeFeatures *uiNewOpenTypeFeatures(void);

// @role uiOpenTypeFeatures destructor
// uiFreeOpenTypeFeatures() frees otf.
uiprivExtern void uiFreeOpenTypeFeatures(uiOpenTypeFeatures *otf);

// uiOpenTypeFeaturesClone() makes a copy of otf and returns it.
// Changing one will not affect the other.
uiprivExtern uiOpenTypeFeatures *uiOpenTypeFeaturesClone(const uiOpenTypeFeatures *otf);

// uiOpenTypeFeaturesAdd() adds the given feature tag and value
// to otf. The feature tag is specified by a, b, c, and d. If there is
// already a value associated with the specified tag in otf, the old
// value is removed.
uiprivExtern void uiOpenTypeFeaturesAdd(uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t value);

// uiOpenTypeFeaturesRemove() removes the given feature tag
// and value from otf. If the tag is not present in otf,
// uiOpenTypeFeaturesRemove() does nothing.
uiprivExtern void uiOpenTypeFeaturesRemove(uiOpenTypeFeatures *otf, char a, char b, char c, char d);

// uiOpenTypeFeaturesGet() determines whether the given feature
// tag is present in otf. If it is, *value is set to the tag's value and
// nonzero is returned. Otherwise, zero is returned.
// 
// Note that if uiOpenTypeFeaturesGet() returns zero, value isn't
// changed. This is important: if a feature is not present in a
// uiOpenTypeFeatures, the feature is NOT treated as if its
// value was zero anyway. Script-specific font shaping rules and
// font-specific feature settings may use a different default value
// for a feature. You should likewise not treat a missing feature as
// having a value of zero either. Instead, a missing feature should
// be treated as having some unspecified default value.
uiprivExtern int uiOpenTypeFeaturesGet(const uiOpenTypeFeatures *otf, char a, char b, char c, char d, uint32_t *value);

// uiOpenTypeFeaturesForEach() executes f for every tag-value
// pair in otf. The enumeration order is unspecified. You cannot
// modify otf while uiOpenTypeFeaturesForEach() is running.
uiprivExtern void uiOpenTypeFeaturesForEach(const uiOpenTypeFeatures *otf, uiOpenTypeFeaturesForEachFunc f, void *data);

// uiNewFeaturesAttribute() creates a new uiAttribute that changes
// the font family of the text it is applied to. otf is copied; you may
// free it after uiNewFeaturesAttribute() returns.
uiprivExtern uiAttribute *uiNewFeaturesAttribute(const uiOpenTypeFeatures *otf);

// uiAttributeFeatures() returns the OpenType features stored in a.
// The returned uiOpenTypeFeatures object is owned by a. It is an
// error to call this on a uiAttribute that does not hold OpenType
// features.
uiprivExtern const uiOpenTypeFeatures *uiAttributeFeatures(const uiAttribute *a);

// uiAttributedString represents a string of UTF-8 text that can
// optionally be embellished with formatting attributes. libui
// provides the list of formatting attributes, which cover common
// formatting traits like boldface and color as well as advanced
// typographical features provided by OpenType like superscripts
// and small caps. These attributes can be combined in a variety of
// ways.
//
// Attributes are applied to runs of Unicode codepoints in the string.
// Zero-length runs are elided. Consecutive runs that have the same
// attribute type and value are merged. Each attribute is independent
// of each other attribute; overlapping attributes of different types
// do not split each other apart, but different values of the same
// attribute type do.
//
// The empty string can also be represented by uiAttributedString,
// but because of the no-zero-length-attribute rule, it will not have
// attributes.
//
// A uiAttributedString takes ownership of all attributes given to
// it, as it may need to duplicate or delete uiAttribute objects at
// any time. By extension, when you free a uiAttributedString,
// all uiAttributes within will also be freed. Each method will
// describe its own rules in more details.
//
// In addition, uiAttributedString provides facilities for moving
// between grapheme clusters, which represent a character
// from the point of view of the end user. The cursor of a text editor
// is always placed on a grapheme boundary, so you can use these
// features to move the cursor left or right by one "character".
// TODO does uiAttributedString itself need this
//
// uiAttributedString does not provide enough information to be able
// to draw itself onto a uiDrawContext or respond to user actions.
// In order to do that, you'll need to use a uiDrawTextLayout, which
// is built from the combination of a uiAttributedString and a set of
// layout-specific properties.
typedef struct uiAttributedString uiAttributedString;

// uiAttributedStringForEachAttributeFunc is the type of the function
// invoked by uiAttributedStringForEachAttribute() for every
// attribute in s. Refer to that function's documentation for more
// details.
typedef uiForEach (*uiAttributedStringForEachAttributeFunc)(const uiAttributedString *s, const uiAttribute *a, size_t start, size_t end, void *data);

// @role uiAttributedString constructor
// uiNewAttributedString() creates a new uiAttributedString from
// initialString. The string will be entirely unattributed.
uiprivExtern uiAttributedString *uiNewAttributedString(const char *initialString);

// @role uiAttributedString destructor
// uiFreeAttributedString() destroys the uiAttributedString s.
// It will also free all uiAttributes within.
uiprivExtern void uiFreeAttributedString(uiAttributedString *s);

// uiAttributedStringString() returns the textual content of s as a
// '\0'-terminated UTF-8 string. The returned pointer is valid until
// the next change to the textual content of s.
uiprivExtern const char *uiAttributedStringString(const uiAttributedString *s);

// uiAttributedStringLength() returns the number of UTF-8 bytes in
// the textual content of s, excluding the terminating '\0'.
uiprivExtern size_t uiAttributedStringLen(const uiAttributedString *s);

// uiAttributedStringAppendUnattributed() adds the '\0'-terminated
// UTF-8 string str to the end of s. The new substring will be
// unattributed.
uiprivExtern void uiAttributedStringAppendUnattributed(uiAttributedString *s, const char *str);

// uiAttributedStringInsertAtUnattributed() adds the '\0'-terminated
// UTF-8 string str to s at the byte position specified by at. The new
// substring will be unattributed; existing attributes will be moved
// along with their text.
uiprivExtern void uiAttributedStringInsertAtUnattributed(uiAttributedString *s, const char *str, size_t at);

// TODO add the Append and InsertAtExtendingAttributes functions
// TODO and add functions that take a string + length

// uiAttributedStringDelete() deletes the characters and attributes of
// s in the byte range [start, end).
uiprivExtern void uiAttributedStringDelete(uiAttributedString *s, size_t start, size_t end);

// TODO add a function to uiAttributedString to get an attribute's value at a specific index or in a specific range, so we can edit

// uiAttributedStringSetAttribute() sets a in the byte range [start, end)
// of s. Any existing attributes in that byte range of the same type are
// removed. s takes ownership of a; you should not use it after
// uiAttributedStringSetAttribute() returns.
uiprivExtern void uiAttributedStringSetAttribute(uiAttributedString *s, uiAttribute *a, size_t start, size_t end);

// uiAttributedStringForEachAttribute() enumerates all the
// uiAttributes in s. It is an error to modify s in f. Within f, s still
// owns the attribute; you can neither free it nor save it for later
// use.
// TODO reword the above for consistency (TODO and find out what I meant by that)
// TODO define an enumeration order (or mark it as undefined); also define how consecutive runs of identical attributes are handled here and sync with the definition of uiAttributedString itself
uiprivExtern void uiAttributedStringForEachAttribute(const uiAttributedString *s, uiAttributedStringForEachAttributeFunc f, void *data);

// TODO const correct this somehow (the implementation needs to mutate the structure)
uiprivExtern size_t uiAttributedStringNumGraphemes(uiAttributedString *s);

// TODO const correct this somehow (the implementation needs to mutate the structure)
uiprivExtern size_t uiAttributedStringByteIndexToGrapheme(uiAttributedString *s, size_t pos);

// TODO const correct this somehow (the implementation needs to mutate the structure)
uiprivExtern size_t uiAttributedStringGraphemeToByteIndex(uiAttributedString *s, size_t pos);

// uiFontDescriptor provides a complete description of a font where
// one is needed. Currently, this means as the default font of a
// uiDrawTextLayout and as the data returned by uiFontButton.
// All the members operate like the respective uiAttributes.
typedef struct uiFontDescriptor uiFontDescriptor;

struct uiFontDescriptor {
	// TODO const-correct this or figure out how to deal with this when getting a value
	char *Family;
	double Size;
	uiTextWeight Weight;
	uiTextItalic Italic;
	uiTextStretch Stretch;
};

// uiDrawTextLayout is a concrete representation of a
// uiAttributedString that can be displayed in a uiDrawContext.
// It includes information important for the drawing of a block of
// text, including the bounding box to wrap the text within, the
// alignment of lines of text within that box, areas to mark as
// being selected, and other things.
//
// Unlike uiAttributedString, the content of a uiDrawTextLayout is
// immutable once it has been created.
//
// TODO talk about OS-specific differences with text drawing that libui can't account for...
typedef struct uiDrawTextLayout uiDrawTextLayout;

// uiDrawTextAlign specifies the alignment of lines of text in a
// uiDrawTextLayout.
// TODO should this really have Draw in the name?
uiprivEnum(uiDrawTextAlign) {
	uiDrawTextAlignLeft,
	uiDrawTextAlignCenter,
	uiDrawTextAlignRight,
};

// uiDrawTextLayoutParams describes a uiDrawTextLayout.
// DefaultFont is used to render any text that is not attributed
// sufficiently in String. Width determines the width of the bounding
// box of the text; the height is determined automatically.
typedef struct uiDrawTextLayoutParams uiDrawTextLayoutParams;

// TODO const-correct this somehow
struct uiDrawTextLayoutParams {
	uiAttributedString *String;
	uiFontDescriptor *DefaultFont;
	double Width;
	uiDrawTextAlign Align;
};

// @role uiDrawTextLayout constructor
// uiDrawNewTextLayout() creates a new uiDrawTextLayout from
// the given parameters.
//
// TODO
// - allow creating a layout out of a substring
// - allow marking compositon strings
// - allow marking selections, even after creation
// - add the following functions:
// 	- uiDrawTextLayoutHeightForWidth() (returns the height that a layout would need to be to display the entire string at a given width)
// 	- uiDrawTextLayoutRangeForSize() (returns what substring would fit in a given size)
// 	- uiDrawTextLayoutNewWithHeight() (limits amount of string used by the height)
// - some function to fix up a range (for text editing)
uiprivExtern uiDrawTextLayout *uiDrawNewTextLayout(uiDrawTextLayoutParams *params);

// @role uiDrawFreeTextLayout destructor
// uiDrawFreeTextLayout() frees tl. The underlying
// uiAttributedString is not freed.
uiprivExtern void uiDrawFreeTextLayout(uiDrawTextLayout *tl);

// uiDrawText() draws tl in c with the top-left point of tl at (x, y).
uiprivExtern void uiDrawText(uiDrawContext *c, uiDrawTextLayout *tl, double x, double y);

// uiDrawTextLayoutExtents() returns the width and height of tl
// in width and height. The returned width may be smaller than
// the width passed into uiDrawNewTextLayout() depending on
// how the text in tl is wrapped. Therefore, you can use this
// function to get the actual size of the text layout.
uiprivExtern void uiDrawTextLayoutExtents(uiDrawTextLayout *tl, double *width, double *height);

// TODO metrics functions

// TODO number of lines visible for clipping rect, range visible for clipping rect?

// uiFontButton is a button that allows users to choose a font when they click on it.
typedef struct uiFontButton uiFontButton;
#define uiFontButton(this) ((uiFontButton *) (this))
// uiFontButtonFont() returns the font currently selected in the uiFontButton in desc.
// uiFontButtonFont() allocates resources in desc; when you are done with the font, call uiFreeFontButtonFont() to release them.
// uiFontButtonFont() does not allocate desc itself; you must do so.
// TODO have a function that sets an entire font descriptor to a range in a uiAttributedString at once, for SetFont?
uiprivExtern void uiFontButtonFont(uiFontButton *b, uiFontDescriptor *desc);
// TOOD SetFont, mechanics
// uiFontButtonOnChanged() sets the function that is called when the font in the uiFontButton is changed.
uiprivExtern void uiFontButtonOnChanged(uiFontButton *b, void (*f)(uiFontButton *, void *), void *data);
// uiNewFontButton() creates a new uiFontButton. The default font selected into the uiFontButton is OS-defined.
uiprivExtern uiFontButton *uiNewFontButton(void);
// uiFreeFontButtonFont() frees resources allocated in desc by uiFontButtonFont().
// After calling uiFreeFontButtonFont(), the contents of desc should be assumed to be undefined (though since you allocate desc itself, you can safely reuse desc for other font descriptors).
// Calling uiFreeFontButtonFont() on a uiFontDescriptor not returned by uiFontButtonFont() results in undefined behavior.
uiprivExtern void uiFreeFontButtonFont(uiFontDescriptor *desc);

uiprivEnum(uiModifiers) {
	uiModifierCtrl = 1 << 0,
	uiModifierAlt = 1 << 1,
	uiModifierShift = 1 << 2,
	uiModifierSuper = 1 << 3,
};

// TODO document drag captures
struct uiAreaMouseEvent {
	// TODO document what these mean for scrolling areas
	double X;
	double Y;

	// TODO see draw above
	double AreaWidth;
	double AreaHeight;

	int Down;
	int Up;

	int Count;

	uiModifiers Modifiers;

	uint64_t Held1To64;
};

uiprivEnum(uiExtKey) {
	uiExtKeyEscape = 1,
	uiExtKeyInsert,			// equivalent to "Help" on Apple keyboards
	uiExtKeyDelete,
	uiExtKeyHome,
	uiExtKeyEnd,
	uiExtKeyPageUp,
	uiExtKeyPageDown,
	uiExtKeyUp,
	uiExtKeyDown,
	uiExtKeyLeft,
	uiExtKeyRight,
	uiExtKeyF1,			// F1..F12 are guaranteed to be consecutive
	uiExtKeyF2,
	uiExtKeyF3,
	uiExtKeyF4,
	uiExtKeyF5,
	uiExtKeyF6,
	uiExtKeyF7,
	uiExtKeyF8,
	uiExtKeyF9,
	uiExtKeyF10,
	uiExtKeyF11,
	uiExtKeyF12,
	uiExtKeyN0,			// numpad keys; independent of Num Lock state
	uiExtKeyN1,			// N0..N9 are guaranteed to be consecutive
	uiExtKeyN2,
	uiExtKeyN3,
	uiExtKeyN4,
	uiExtKeyN5,
	uiExtKeyN6,
	uiExtKeyN7,
	uiExtKeyN8,
	uiExtKeyN9,
	uiExtKeyNDot,
	uiExtKeyNEnter,
	uiExtKeyNAdd,
	uiExtKeyNSubtract,
	uiExtKeyNMultiply,
	uiExtKeyNDivide,
};

struct uiAreaKeyEvent {
	char Key;
	uiExtKey ExtKey;
	uiModifiers Modifier;

	uiModifiers Modifiers;

	int Up;
};

typedef struct uiColorButton uiColorButton;
#define uiColorButton(this) ((uiColorButton *) (this))
uiprivExtern void uiColorButtonColor(uiColorButton *b, double *r, double *g, double *bl, double *a);
uiprivExtern void uiColorButtonSetColor(uiColorButton *b, double r, double g, double bl, double a);
uiprivExtern void uiColorButtonOnChanged(uiColorButton *b, void (*f)(uiColorButton *, void *), void *data);
uiprivExtern uiColorButton *uiNewColorButton(void);

typedef struct uiForm uiForm;
#define uiForm(this) ((uiForm *) (this))
uiprivExtern void uiFormAppend(uiForm *f, const char *label, uiControl *c, int stretchy);
uiprivExtern void uiFormDelete(uiForm *f, int index);
uiprivExtern int uiFormPadded(uiForm *f);
uiprivExtern void uiFormSetPadded(uiForm *f, int padded);
uiprivExtern uiForm *uiNewForm(void);

uiprivEnum(uiAlign) {
	uiAlignFill,
	uiAlignStart,
	uiAlignCenter,
	uiAlignEnd,
};

uiprivEnum(uiAt) {
	uiAtLeading,
	uiAtTop,
	uiAtTrailing,
	uiAtBottom,
};

typedef struct uiGrid uiGrid;
#define uiGrid(this) ((uiGrid *) (this))
uiprivExtern void uiGridAppend(uiGrid *g, uiControl *c, int left, int top, int xspan, int yspan, int hexpand, uiAlign halign, int vexpand, uiAlign valign);
uiprivExtern void uiGridInsertAt(uiGrid *g, uiControl *c, uiControl *existing, uiAt at, int xspan, int yspan, int hexpand, uiAlign halign, int vexpand, uiAlign valign);
uiprivExtern int uiGridPadded(uiGrid *g);
uiprivExtern void uiGridSetPadded(uiGrid *g, int padded);
uiprivExtern uiGrid *uiNewGrid(void);

// uiImage stores an image for display on screen.
// 
// Images are built from one or more representations, each with the
// same aspect ratio but a different pixel size. libui automatically
// selects the most appropriate representation for drawing the image
// when it comes time to draw the image; what this means depends
// on the pixel density of the target context. Therefore, one can use
// uiImage to draw higher-detailed images on higher-density
// displays. The typical use cases are either:
// 
// 	- have just a single representation, at which point all screens
// 	  use the same image, and thus uiImage acts like a simple
// 	  bitmap image, or
// 	- have two images, one at normal resolution and one at 2x
// 	  resolution; this matches the current expectations of some
// 	  desktop systems at the time of writing (mid-2018)
// 
// uiImage is very simple: it only supports premultiplied 32-bit
// RGBA images, and libui does not provide any image file loading
// or image format conversion utilities on top of that.
typedef struct uiImage uiImage;

// @role uiImage constructor
// uiNewImage creates a new uiImage with the given width and
// height. This width and height should be the size in points of the
// image in the device-independent case; typically this is the 1x size.
// TODO for all uiImage functions: use const void * for const correctness
uiprivExtern uiImage *uiNewImage(double width, double height);

// @role uiImage destructor
// uiFreeImage frees the given image and all associated resources.
uiprivExtern void uiFreeImage(uiImage *i);

// uiImageAppend adds a representation to the uiImage.
// pixels should point to a byte array of premultiplied pixels
// stored in [R G B A] order (so ((uint8_t *) pixels)[0] is the R of the
// first pixel and [3] is the A of the first pixel). pixelWidth and
// pixelHeight is the size *in pixels* of the image, and pixelStride is
// the number *of bytes* per row of the pixels array. Therefore,
// pixels itself must be at least byteStride * pixelHeight bytes long.
// TODO see if we either need the stride or can provide a way to get the OS-preferred stride (in cairo we do)
uiprivExtern void uiImageAppend(uiImage *i, void *pixels, int pixelWidth, int pixelHeight, int byteStride);

// uiTableValue stores a value to be passed along uiTable and
// uiTableModel.
//
// You do not create uiTableValues directly; instead, you create a
// uiTableValue of a given type using the specialized constructor
// functions.
//
// uiTableValues are immutable and the uiTableModel and uiTable
// take ownership of the uiTableValue object once returned, copying
// its contents as necessary.
typedef struct uiTableValue uiTableValue;

// @role uiTableValue destructor
// uiFreeTableValue() frees a uiTableValue. You generally do not
// need to call this yourself, as uiTable and uiTableModel do this
// for you. In fact, it is an error to call this function on a uiTableValue
// that has been given to a uiTable or uiTableModel. You can call this,
// however, if you created a uiTableValue that you aren't going to
// use later, or if you called a uiTableModelHandler method directly
// and thus never transferred ownership of the uiTableValue.
uiprivExtern void uiFreeTableValue(uiTableValue *v);

// uiTableValueType holds the possible uiTableValue types that may
// be returned by uiTableValueGetType(). Refer to the documentation
// for each type's constructor function for details on each type.
// TODO actually validate these
uiprivEnum(uiTableValueType) {
	uiTableValueTypeString,
	uiTableValueTypeImage,
	uiTableValueTypeInt,
	uiTableValueTypeColor,
};

// uiTableValueGetType() returns the type of v.
// TODO I don't like this name
uiprivExtern uiTableValueType uiTableValueGetType(const uiTableValue *v);

// uiNewTableValueString() returns a new uiTableValue that contains
// str. str is copied; you do not need to keep it alive after
// uiNewTableValueString() returns.
uiprivExtern uiTableValue *uiNewTableValueString(const char *str);

// uiTableValueString() returns the string stored in v. The returned
// string is owned by v. It is an error to call this on a uiTableValue
// that does not hold a string.
uiprivExtern const char *uiTableValueString(const uiTableValue *v);

// uiNewTableValueImage() returns a new uiTableValue that contains
// the given uiImage.
// 
// Unlike other similar constructors, uiNewTableValueImage() does
// NOT copy the image. This is because images are comparatively
// larger than the other objects in question. Therefore, you MUST
// keep the image alive as long as the returned uiTableValue is alive.
// As a general rule, if libui calls a uiTableModelHandler method, the
// uiImage is safe to free once any of your code is once again
// executed.
uiprivExtern uiTableValue *uiNewTableValueImage(uiImage *img);

// uiTableValueImage() returns the uiImage stored in v. As these
// images are not owned by v, you should not assume anything
// about the lifetime of the image (unless you created the image,
// and thus control its lifetime). It is an error to call this on a
// uiTableValue that does not hold an image.
uiprivExtern uiImage *uiTableValueImage(const uiTableValue *v);

// uiNewTableValueInt() returns a uiTableValue that stores the given
// int. This can be used both for boolean values (nonzero is true, as
// in C) or progresses (in which case the valid range is -1..100
// inclusive).
uiprivExtern uiTableValue *uiNewTableValueInt(int i);

// uiTableValueInt() returns the int stored in v. It is an error to call
// this on a uiTableValue that does not store an int.
uiprivExtern int uiTableValueInt(const uiTableValue *v);

// uiNewTableValueColor() returns a uiTableValue that stores the
// given color.
uiprivExtern uiTableValue *uiNewTableValueColor(double r, double g, double b, double a);

// uiTableValueColor() returns the color stored in v. It is an error to
// call this on a uiTableValue that does not store a color.
// TODO define whether all this, for both uiTableValue and uiAttribute, is undefined behavior or a caught error
uiprivExtern void uiTableValueColor(const uiTableValue *v, double *r, double *g, double *b, double *a);

// uiTableModel is an object that provides the data for a uiTable.
// This data is returned via methods you provide in the
// uiTableModelHandler struct.
//
// uiTableModel represents data using a table, but this table does
// not map directly to uiTable itself. Instead, you can have data
// columns which provide instructions for how to render a given
// uiTable's column — for instance, one model column can be used
// to give certain rows of a uiTable a different background color.
// Row numbers DO match with uiTable row numbers.
//
// Once created, the number and data types of columns of a
// uiTableModel cannot change.
//
// Row and column numbers start at 0. A uiTableModel can be
// associated with more than one uiTable at a time.
typedef struct uiTableModel uiTableModel;

// uiTableModelHandler defines the methods that uiTableModel
// calls when it needs data. Once a uiTableModel is created, these
// methods cannot change.
typedef struct uiTableModelHandler uiTableModelHandler;

// TODO validate ranges; validate types on each getter/setter call (? table columns only?)
struct uiTableModelHandler {
	// NumColumns returns the number of model columns in the
	// uiTableModel. This value must remain constant through the
	// lifetime of the uiTableModel. This method is not guaranteed
	// to be called depending on the system.
	// TODO strongly check column numbers and types on all platforms so these clauses can go away
	int (*NumColumns)(uiTableModelHandler *, uiTableModel *);
	// ColumnType returns the value type of the data stored in
	// the given model column of the uiTableModel. The returned
	// values must remain constant through the lifetime of the
	// uiTableModel. This method is not guaranteed to be called
	// depending on the system.
	uiTableValueType (*ColumnType)(uiTableModelHandler *, uiTableModel *, int);
	// NumRows returns the number or rows in the uiTableModel.
	// This value must be non-negative.
	int (*NumRows)(uiTableModelHandler *, uiTableModel *);
	// CellValue returns a uiTableValue corresponding to the model
	// cell at (row, column). The type of the returned uiTableValue
	// must match column's value type. Under some circumstances,
	// NULL may be returned; refer to the various methods that add
	// columns to uiTable for details. Once returned, the uiTable
	// that calls CellValue will free the uiTableValue returned.
	uiTableValue *(*CellValue)(uiTableModelHandler *mh, uiTableModel *m, int row, int column);
	// SetCellValue changes the model cell value at (row, column)
	// in the uiTableModel. Within this function, either do nothing
	// to keep the current cell value or save the new cell value as
	// appropriate. After SetCellValue is called, the uiTable will
	// itself reload the table cell. Under certain conditions, the
	// uiTableValue passed in can be NULL; refer to the various
	// methods that add columns to uiTable for details. Once
	// returned, the uiTable that called SetCellValue will free the
	// uiTableValue passed in.
	void (*SetCellValue)(uiTableModelHandler *, uiTableModel *, int, int, const uiTableValue *);
};

// @role uiTableModel constructor
// uiNewTableModel() creates a new uiTableModel with the given
// handler methods.
uiprivExtern uiTableModel *uiNewTableModel(uiTableModelHandler *mh);

// @role uiTableModel destructor
// uiFreeTableModel() frees the given table model. It is an error to
// free table models currently associated with a uiTable.
uiprivExtern void uiFreeTableModel(uiTableModel *m);

// uiTableModelRowInserted() tells any uiTable associated with m
// that a new row has been added to m at index index. You call
// this function when the number of rows in your model has
// changed; after calling it, NumRows() should returm the new row
// count.
uiprivExtern void uiTableModelRowInserted(uiTableModel *m, int newIndex);

// uiTableModelRowChanged() tells any uiTable associated with m
// that the data in the row at index has changed. You do not need to
// call this in your SetCellValue() handlers, but you do need to call
// this if your data changes at some other point.
uiprivExtern void uiTableModelRowChanged(uiTableModel *m, int index);

// uiTableModelRowDeleted() tells any uiTable associated with m
// that the row at index index has been deleted. You call this
// function when the number of rows in your model has changed;
// after calling it, NumRows() should returm the new row
// count.
// TODO for this and Inserted: make sure the "after" part is right; clarify if it's after returning or after calling
uiprivExtern void uiTableModelRowDeleted(uiTableModel *m, int oldIndex);
// TODO reordering/moving

// uiTableModelColumnNeverEditable and
// uiTableModelColumnAlwaysEditable are the value of an editable
// model column parameter to one of the uiTable create column
// functions; if used, that jparticular uiTable colum is not editable
// by the user and always editable by the user, respectively.
#define uiTableModelColumnNeverEditable (-1)
#define uiTableModelColumnAlwaysEditable (-2)

// uiTableTextColumnOptionalParams are the optional parameters
// that control the appearance of the text column of a uiTable.
typedef struct uiTableTextColumnOptionalParams uiTableTextColumnOptionalParams;

// uiTableParams defines the parameters passed to uiNewTable().
typedef struct uiTableParams uiTableParams;

struct uiTableTextColumnOptionalParams {
	// ColorModelColumn is the model column containing the
	// text color of this uiTable column's text, or -1 to use the
	// default color.
	//
	// If CellValue() for this column for any cell returns NULL, that
	// cell will also use the default text color.
	int ColorModelColumn;
};

struct uiTableParams {
	// Model is the uiTableModel to use for this uiTable.
	// This parameter cannot be NULL.
	uiTableModel *Model;
	// RowBackgroundColorModelColumn is a model column
	// number that defines the background color used for the
	// entire row in the uiTable, or -1 to use the default color for
	// all rows.
	//
	// If CellValue() for this column for any row returns NULL, that
	// row will also use the default background color.
	int RowBackgroundColorModelColumn;
};

// uiTable is a uiControl that shows tabular data, allowing users to
// manipulate rows of such data at a time.
typedef struct uiTable uiTable;
#define uiTable(this) ((uiTable *) (this))

// uiTableAppendTextColumn() appends a text column to t.
// name is displayed in the table header.
// textModelColumn is where the text comes from.
// If a row is editable according to textEditableModelColumn,
// SetCellValue() is called with textModelColumn as the column.
uiprivExtern void uiTableAppendTextColumn(uiTable *t,
	const char *name,
	int textModelColumn,
	int textEditableModelColumn,
	uiTableTextColumnOptionalParams *textParams);

// uiTableAppendImageColumn() appends an image column to t.
// Images are drawn at icon size, appropriate to the pixel density
// of the screen showing the uiTable.
uiprivExtern void uiTableAppendImageColumn(uiTable *t,
	const char *name,
	int imageModelColumn);

// uiTableAppendImageTextColumn() appends a column to t that
// shows both an image and text.
uiprivExtern void uiTableAppendImageTextColumn(uiTable *t,
	const char *name,
	int imageModelColumn,
	int textModelColumn,
	int textEditableModelColumn,
	uiTableTextColumnOptionalParams *textParams);

// uiTableAppendCheckboxColumn appends a column to t that
// contains a checkbox that the user can interact with (assuming the
// checkbox is editable). SetCellValue() will be called with
// checkboxModelColumn as the column in this case.
uiprivExtern void uiTableAppendCheckboxColumn(uiTable *t,
	const char *name,
	int checkboxModelColumn,
	int checkboxEditableModelColumn);

// uiTableAppendCheckboxTextColumn() appends a column to t
// that contains both a checkbox and text.
uiprivExtern void uiTableAppendCheckboxTextColumn(uiTable *t,
	const char *name,
	int checkboxModelColumn,
	int checkboxEditableModelColumn,
	int textModelColumn,
	int textEditableModelColumn,
	uiTableTextColumnOptionalParams *textParams);

// uiTableAppendProgressBarColumn() appends a column to t
// that displays a progress bar. These columns work like
// uiProgressBar: a cell value of 0..100 displays that percentage, and
// a cell value of -1 displays an indeterminate progress bar.
uiprivExtern void uiTableAppendProgressBarColumn(uiTable *t,
	const char *name,
	int progressModelColumn);

// uiTableAppendButtonColumn() appends a column to t
// that shows a button that the user can click on. When the user
// does click on the button, SetCellValue() is called with a NULL
// value and buttonModelColumn as the column.
// CellValue() on buttonModelColumn should return the text to show
// in the button.
uiprivExtern void uiTableAppendButtonColumn(uiTable *t,
	const char *name,
	int buttonModelColumn,
	int buttonClickableModelColumn);

// uiNewTable() creates a new uiTable with the specified parameters.
uiprivExtern uiTable *uiNewTable(uiTableParams *params);
