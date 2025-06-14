/* See LICENSE file for copyright and license details. */

#include "X11/keysym.h"
#include "X11/XF86keysym.h"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 4;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "0xProto Nerd Font:size=9" };
static const char dmenufont[]       = "0xProto Nerd Font:size=9";

#include "/home/think/.cache/wal/my_colors.h"

/*static char normbgcolor[]           = "#222222";*/
/*static char normbordercolor[]       = "#444444";*/
/*static char normfgcolor[]           = "#bbbbbb";*/
/*static char selfgcolor[]            = "#eeeeee";*/
/*static const char urgbordercolor[]  = "#ff0000";*/
/*static char selbordercolor[]        = "#005577";*/
/*static char selbgcolor[]            = "#005577";*/
/*static char *colors[][3] = {*/
/*                      fg           bg           border   */
/*       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },*/
/*       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },*/
/*};*/

/* tagging */
static const char *tags[] = { "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  " };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "zen-browser", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st-256color",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "|  ",      tile },    /* first entry is default */
	{ "|  ",      NULL },    /* no layout function means floating behavior */
	{ "|  ",      monocle },
 	{ "|  ",      spiral },
 	{ "|  ",      dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-fn", dmenufont, "-nb", norm_bg, "-nf", norm_fg,
"-sb", sel_border, "-sf", sel_fg, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *brightness_up[]   = { "brightnessctl", "set", "1%+", NULL };
static const char *brightness_down[] = { "brightnessctl", "set", "1%-", NULL };
static const char *vol_up[]   = { "pamixer", "--increase", "1", NULL };
static const char *vol_down[] = { "pamixer", "--decrease", "1", NULL };
static const char *vol_mute[] = { "pamixer", "--toggle-mute", NULL };
static const char *redshift_toggle[] = { "redshift", "-x", "&&", "redshift", "-l", "30.7363:76.7884", NULL };
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_z,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_n,      spawn,          SHCMD("st -e newsboat") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
  { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = (const char*[]){ "kitty", NULL } } },
  { MODKEY,                       XK_p,      spawn,          {.v = (const char*[]){ "zen-browser", NULL } } },
  { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = (const char*[]){ "qutebrowser", NULL } } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_r,      spawn,          {.v = redshift_toggle } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
  { MODKEY,                       XK_x,      setlayout,      {.v = &layouts[3]} }, // Spiral mode
  { MODKEY,                       XK_c,      setlayout,      {.v = &layouts[4]} }, // Dwindle mode
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
/*                          scripts                            */
  { MODKEY,                       XK_u,      spawn,          SHCMD("~/scripts/open-url.sh") },
  { MODKEY,                       XK_w,      spawn,          SHCMD("~/.local/bin/dwmstyle") },
  { MODKEY,                       XK_s,      spawn,          SHCMD("~/.local/bin/share") },

  { MODKEY|ShiftMask,  XK_End,      spawn,          SHCMD("systemctl poweroff") },   // Shutdown
  { MODKEY|ShiftMask,  XK_Home,     spawn,          SHCMD("systemctl reboot") },    // Reboot
  { MODKEY|ShiftMask,  XK_Insert,   spawn,          SHCMD("systemctl hibernate") }, // Hibernate
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { 0, XF86XK_MonBrightnessDown, spawn, {.v = brightness_down } },
  { 0, XF86XK_MonBrightnessUp,   spawn, {.v = brightness_up } },
  { 0, XF86XK_AudioLowerVolume, spawn, {.v = vol_down } },
  { 0, XF86XK_AudioRaiseVolume, spawn, {.v = vol_up } },
  { 0, XF86XK_AudioMute,        spawn, {.v = vol_mute } },
  /* Screenshot (full screen) */
{ 0, XK_Print, spawn, SHCMD("flameshot full -p ~/pix/Screenshot/") },
/* Screenshot (interactive selection) */
{ ShiftMask, XK_Print, spawn, SHCMD("flameshot gui") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

