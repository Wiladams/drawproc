#pragma once

#define MWKEY_UNKNOWN		0
/* Following special control keysyms are mapped to ASCII*/
#define MWKEY_BACKSPACE		8
#define MWKEY_TAB		9
#define MWKEY_ENTER		13
#define MWKEY_ESCAPE		27
/* Keysyms from 32-126 are mapped to ASCII*/

#define MWKEY_NONASCII_MASK	0xFF00
/* Following keysyms are mapped to private use portion of Unicode-16*/
/* arrows + home/end pad*/
#define MWKEY_FIRST		0xF800
#define MWKEY_LEFT		0xF800
#define MWKEY_RIGHT		0xF801
#define MWKEY_UP		0xF802
#define MWKEY_DOWN		0xF803
#define MWKEY_INSERT		0xF804
#define MWKEY_DELETE		0xF805
#define MWKEY_HOME		0xF806
#define MWKEY_END		0xF807
#define MWKEY_PAGEUP		0xF808
#define MWKEY_PAGEDOWN		0xF809

/* Numeric keypad*/
#define MWKEY_KP0		0xF80A
#define MWKEY_KP1		0xF80B
#define MWKEY_KP2		0xF80C
#define MWKEY_KP3		0xF80D
#define MWKEY_KP4		0xF80E
#define MWKEY_KP5		0xF80F
#define MWKEY_KP6		0xF810
#define MWKEY_KP7		0xF811
#define MWKEY_KP8		0xF812
#define MWKEY_KP9		0xF813
#define MWKEY_KP_PERIOD		0xF814
#define MWKEY_KP_DIVIDE		0xF815
#define MWKEY_KP_MULTIPLY	0xF816
#define MWKEY_KP_MINUS		0xF817
#define MWKEY_KP_PLUS		0xF818
#define MWKEY_KP_ENTER		0xF819
#define MWKEY_KP_EQUALS		0xF81A

/* Function keys */
#define MWKEY_F1		0xF81B
#define MWKEY_F2		0xF81C
#define MWKEY_F3		0xF81D
#define MWKEY_F4		0xF81E
#define MWKEY_F5		0xF81F
#define MWKEY_F6		0xF820
#define MWKEY_F7		0xF821
#define MWKEY_F8		0xF822
#define MWKEY_F9		0xF823
#define MWKEY_F10		0xF824
#define MWKEY_F11		0xF825
#define MWKEY_F12		0xF827

/* Key state modifier keys*/
#define MWKEY_NUMLOCK		0xF828
#define MWKEY_CAPSLOCK		0xF829
#define MWKEY_SCROLLOCK		0xF82A
#define MWKEY_LSHIFT		0xF82B
#define MWKEY_RSHIFT		0xF82C
#define MWKEY_LCTRL		0xF82D
#define MWKEY_RCTRL		0xF82E
#define MWKEY_LALT		0xF82F
#define MWKEY_RALT		0xF830
#define MWKEY_LMETA		0xF831
#define MWKEY_RMETA		0xF832
#define MWKEY_ALTGR		0xF833

/* Misc function keys*/
#define MWKEY_PRINT		0xF834
#define MWKEY_SYSREQ		0xF835
#define MWKEY_PAUSE		0xF836
#define MWKEY_BREAK		0xF837
#define MWKEY_QUIT		0xF838	/* virtual key*/
#define MWKEY_MENU		0xF839	/* virtual key*/
#define MWKEY_REDRAW		0xF83A	/* virtual key*/

/* Handheld function keys*/
/* #define MWKEY_RECORD		0xF840 -- Replaced by HAVi code */
/* #define MWKEY_PLAY		0xF841 -- Replaced by HAVi code */
#define MWKEY_CONTRAST		0xF842
#define MWKEY_BRIGHTNESS	0xF843
#define MWKEY_SELECTUP		0xF844
#define MWKEY_SELECTDOWN	0xF845
#define MWKEY_ACCEPT		0xF846
#define MWKEY_CANCEL		0xF847
#define MWKEY_APP1		0xF848
#define MWKEY_APP2		0xF849
#define MWKEY_APP3              0xF84A
#define MWKEY_APP4              0xF84B
#define MWKEY_SUSPEND           0xF84C
#define MWKEY_END_NORMAL	0xF84D	/* insert additional keys before this*/

/*
* The following keys are useful for remote controls on consumer
* electronics devices (e.g. TVs, videos, DVD players, cable
* boxes, satellite boxes, digital terrestrial recievers, ...)
*
* The codes are taken from the HAVi specification:
*   HAVi Level 2 User Interface version 1.1, May 15th 2001
* They are listed in section 8.7.
*
* For more information see http://www.havi.org/
*/

#define MWKEY_HAVI_KEY_BASE   (MWKEY_END_NORMAL+1) /* MWKEY code for first HAVi key */
#define MWKEY_HAVI_CODE_FIRST  403			/* HAVi code for first HAVi key */
#define MWKEY_HAVI_CODE_LAST   460			/* HAVi code for last HAVi key */
/* HRcEvent.VK_... code to MWKEY_... code */
#define MWKEY_FROM_HAVI_CODE(h) ((h) + (MWKEY_HAVI_KEY_BASE - MWKEY_HAVI_CODE_FIRST))
/* MWKEY_... code to HRcEvent.VK_... code */
#define MWKEY_TO_HAVI_CODE(m)   ((m) - (MWKEY_HAVI_KEY_BASE - MWKEY_HAVI_CODE_FIRST))
/* Can an MWKEY_... code be converted into a HRcEvent.VK_... code? */
#define MWKEY_IS_HAVI_CODE(m)   (  (unsigned)((m) - MWKEY_HAVI_KEY_BASE) \
               <= (unsigned)(MWKEY_HAVI_CODE_LAST - MWKEY_HAVI_CODE_FIRST) )

#define MWKEY_COLORED_KEY_0         MWKEY_FROM_HAVI_CODE(403)
#define MWKEY_COLORED_KEY_1         MWKEY_FROM_HAVI_CODE(404)
#define MWKEY_COLORED_KEY_2         MWKEY_FROM_HAVI_CODE(405)
#define MWKEY_COLORED_KEY_3         MWKEY_FROM_HAVI_CODE(406)
#define MWKEY_COLORED_KEY_4         MWKEY_FROM_HAVI_CODE(407)
#define MWKEY_COLORED_KEY_5         MWKEY_FROM_HAVI_CODE(408)
#define MWKEY_POWER                 MWKEY_FROM_HAVI_CODE(409)
#define MWKEY_DIMMER                MWKEY_FROM_HAVI_CODE(410)
#define MWKEY_WINK                  MWKEY_FROM_HAVI_CODE(411)
#define MWKEY_REWIND                MWKEY_FROM_HAVI_CODE(412)
#define MWKEY_STOP                  MWKEY_FROM_HAVI_CODE(413)
#define MWKEY_EJECT_TOGGLE          MWKEY_FROM_HAVI_CODE(414)
#define MWKEY_PLAY                  MWKEY_FROM_HAVI_CODE(415)
#define MWKEY_RECORD                MWKEY_FROM_HAVI_CODE(416)
#define MWKEY_FAST_FWD              MWKEY_FROM_HAVI_CODE(417)
#define MWKEY_PLAY_SPEED_UP         MWKEY_FROM_HAVI_CODE(418)
#define MWKEY_PLAY_SPEED_DOWN       MWKEY_FROM_HAVI_CODE(419)
#define MWKEY_PLAY_SPEED_RESET      MWKEY_FROM_HAVI_CODE(420)
#define MWKEY_RECORD_SPEED_NEXT     MWKEY_FROM_HAVI_CODE(421)
#define MWKEY_GO_TO_START           MWKEY_FROM_HAVI_CODE(422)
#define MWKEY_GO_TO_END             MWKEY_FROM_HAVI_CODE(423)
#define MWKEY_TRACK_PREV            MWKEY_FROM_HAVI_CODE(424)
#define MWKEY_TRACK_NEXT            MWKEY_FROM_HAVI_CODE(425)
#define MWKEY_RANDOM_TOGGLE         MWKEY_FROM_HAVI_CODE(426)
#define MWKEY_CHANNEL_UP            MWKEY_FROM_HAVI_CODE(427)
#define MWKEY_CHANNEL_DOWN          MWKEY_FROM_HAVI_CODE(428)
#define MWKEY_STORE_FAVORITE_0      MWKEY_FROM_HAVI_CODE(429)
#define MWKEY_STORE_FAVORITE_1      MWKEY_FROM_HAVI_CODE(430)
#define MWKEY_STORE_FAVORITE_2      MWKEY_FROM_HAVI_CODE(431)
#define MWKEY_STORE_FAVORITE_3      MWKEY_FROM_HAVI_CODE(432)
#define MWKEY_RECALL_FAVORITE_0     MWKEY_FROM_HAVI_CODE(433)
#define MWKEY_RECALL_FAVORITE_1     MWKEY_FROM_HAVI_CODE(434)
#define MWKEY_RECALL_FAVORITE_2     MWKEY_FROM_HAVI_CODE(435)
#define MWKEY_RECALL_FAVORITE_3     MWKEY_FROM_HAVI_CODE(436)
#define MWKEY_CLEAR_FAVORITE_0      MWKEY_FROM_HAVI_CODE(437)
#define MWKEY_CLEAR_FAVORITE_1      MWKEY_FROM_HAVI_CODE(438)
#define MWKEY_CLEAR_FAVORITE_2      MWKEY_FROM_HAVI_CODE(439)
#define MWKEY_CLEAR_FAVORITE_3      MWKEY_FROM_HAVI_CODE(440)
#define MWKEY_SCAN_CHANNELS_TOGGLE  MWKEY_FROM_HAVI_CODE(441)
#define MWKEY_PINP_TOGGLE           MWKEY_FROM_HAVI_CODE(442)
#define MWKEY_SPLIT_SCREEN_TOGGLE   MWKEY_FROM_HAVI_CODE(443)
#define MWKEY_DISPLAY_SWAP          MWKEY_FROM_HAVI_CODE(444)
#define MWKEY_SCREEN_MODE_NEXT      MWKEY_FROM_HAVI_CODE(445)
#define MWKEY_VIDEO_MODE_NEXT       MWKEY_FROM_HAVI_CODE(446)
#define MWKEY_VOLUME_UP             MWKEY_FROM_HAVI_CODE(447)
#define MWKEY_VOLUME_DOWN           MWKEY_FROM_HAVI_CODE(448)
#define MWKEY_MUTE                  MWKEY_FROM_HAVI_CODE(449)
#define MWKEY_SURROUND_MODE_NEXT    MWKEY_FROM_HAVI_CODE(450)
#define MWKEY_BALANCE_RIGHT         MWKEY_FROM_HAVI_CODE(451)
#define MWKEY_BALANCE_LEFT          MWKEY_FROM_HAVI_CODE(452)
#define MWKEY_FADER_FRONT           MWKEY_FROM_HAVI_CODE(453)
#define MWKEY_FADER_REAR            MWKEY_FROM_HAVI_CODE(454)
#define MWKEY_BASS_BOOST_UP         MWKEY_FROM_HAVI_CODE(455)
#define MWKEY_BASS_BOOST_DOWN       MWKEY_FROM_HAVI_CODE(456)
#define MWKEY_INFO                  MWKEY_FROM_HAVI_CODE(457)
#define MWKEY_GUIDE                 MWKEY_FROM_HAVI_CODE(458)
#define MWKEY_TELETEXT              MWKEY_FROM_HAVI_CODE(459)
#define MWKEY_SUBTITLE              MWKEY_FROM_HAVI_CODE(460)

#define MWKEY_LAST                  MWKEY_SUBTITLE

/* Keyboard state modifiers*/
#define MWKMOD_NONE  		0x0000
#define MWKMOD_LSHIFT		0x0001
#define MWKMOD_RSHIFT		0x0002
#define MWKMOD_LCTRL 		0x0040
#define MWKMOD_RCTRL 		0x0080
#define MWKMOD_LALT  		0x0100
#define MWKMOD_RALT  		0x0200
#define MWKMOD_LMETA 		0x0400		/* Windows key*/
#define MWKMOD_RMETA 		0x0800		/* Windows key*/
#define MWKMOD_NUM   		0x1000
#define MWKMOD_CAPS  		0x2000
#define MWKMOD_ALTGR 		0x4000
#define MWKMOD_SCR			0x8000

#define MWKMOD_CTRL	(MWKMOD_LCTRL|MWKMOD_RCTRL)
#define MWKMOD_SHIFT	(MWKMOD_LSHIFT|MWKMOD_RSHIFT)
#define MWKMOD_ALT	(MWKMOD_LALT|MWKMOD_RALT)
#define MWKMOD_META	(MWKMOD_LMETA|MWKMOD_RMETA)

typedef struct {
	int led;
	int led_mode;
} MWKBINFO, *PMWKBINFO;

#define MWKINFO_LED_MASK	(1 << 0)
#define MWKINFO_LED_MODE_MASK	(1 << 1)

/* Keyboard info values */
#define MWKINFO_LED_CAP		(1 << 0)
#define MWKINFO_LED_NUM		(1 << 1)
#define MWKINFO_LED_SCR		(1 << 2)
#define MWKINFO_LED_MODE_ON	(1 << 3)
#define MWKINFO_LED_MODE_OFF (1 << 4)
