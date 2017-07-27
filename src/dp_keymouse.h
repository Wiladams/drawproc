#pragma once

#define DPKEY_UNKNOWN		0
/* Following special control keysyms are mapped to ASCII*/
#define DPKEY_BACKSPACE		8
#define DPKEY_TAB		9
#define DPKEY_ENTER		13
#define DPKEY_ESCAPE		27
/* Keysyms from 32-126 are mapped to ASCII*/

#define DPKEY_NONASCII_MASK	0xFF00
/* Following keysyms are mapped to private use portion of Unicode-16*/
/* arrows + home/end pad*/
#define DPKEY_FIRST		0xF800
#define DPKEY_LEFT		0xF800
#define DPKEY_RIGHT		0xF801
#define DPKEY_UP		0xF802
#define DPKEY_DOWN		0xF803
#define DPKEY_INSERT		0xF804
#define DPKEY_DELETE		0xF805
#define DPKEY_HOME		0xF806
#define DPKEY_END		0xF807
#define DPKEY_PAGEUP		0xF808
#define DPKEY_PAGEDOWN		0xF809

/* Numeric keypad*/
#define DPKEY_KP0		0xF80A
#define DPKEY_KP1		0xF80B
#define DPKEY_KP2		0xF80C
#define DPKEY_KP3		0xF80D
#define DPKEY_KP4		0xF80E
#define DPKEY_KP5		0xF80F
#define DPKEY_KP6		0xF810
#define DPKEY_KP7		0xF811
#define DPKEY_KP8		0xF812
#define DPKEY_KP9		0xF813
#define DPKEY_KP_PERIOD		0xF814
#define DPKEY_KP_DIVIDE		0xF815
#define DPKEY_KP_MULTIPLY	0xF816
#define DPKEY_KP_MINUS		0xF817
#define DPKEY_KP_PLUS		0xF818
#define DPKEY_KP_ENTER		0xF819
#define DPKEY_KP_EQUALS		0xF81A

/* Function keys */
#define DPKEY_F1		0xF81B
#define DPKEY_F2		0xF81C
#define DPKEY_F3		0xF81D
#define DPKEY_F4		0xF81E
#define DPKEY_F5		0xF81F
#define DPKEY_F6		0xF820
#define DPKEY_F7		0xF821
#define DPKEY_F8		0xF822
#define DPKEY_F9		0xF823
#define DPKEY_F10		0xF824
#define DPKEY_F11		0xF825
#define DPKEY_F12		0xF827

/* Key state modifier keys*/
#define DPKEY_NUMLOCK		0xF828
#define DPKEY_CAPSLOCK		0xF829
#define DPKEY_SCROLLOCK		0xF82A
#define DPKEY_LSHIFT		0xF82B
#define DPKEY_RSHIFT		0xF82C
#define DPKEY_LCTRL		0xF82D
#define DPKEY_RCTRL		0xF82E
#define DPKEY_LALT		0xF82F
#define DPKEY_RALT		0xF830
#define DPKEY_LMETA		0xF831
#define DPKEY_RMETA		0xF832
#define DPKEY_ALTGR		0xF833

/* Misc function keys*/
#define DPKEY_PRINT		0xF834
#define DPKEY_SYSREQ		0xF835
#define DPKEY_PAUSE		0xF836
#define DPKEY_BREAK		0xF837
#define DPKEY_QUIT		0xF838	/* virtual key*/
#define DPKEY_MENU		0xF839	/* virtual key*/
#define DPKEY_REDRAW		0xF83A	/* virtual key*/

/* Handheld function keys*/
/* #define DPKEY_RECORD		0xF840 -- Replaced by HAVi code */
/* #define DPKEY_PLAY		0xF841 -- Replaced by HAVi code */
#define DPKEY_CONTRAST		0xF842
#define DPKEY_BRIGHTNESS	0xF843
#define DPKEY_SELECTUP		0xF844
#define DPKEY_SELECTDOWN	0xF845
#define DPKEY_ACCEPT		0xF846
#define DPKEY_CANCEL		0xF847
#define DPKEY_APP1		0xF848
#define DPKEY_APP2		0xF849
#define DPKEY_APP3              0xF84A
#define DPKEY_APP4              0xF84B
#define DPKEY_SUSPEND           0xF84C
#define DPKEY_END_NORMAL	0xF84D	// insert additional keys before this

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

#define DPKEY_HAVI_KEY_BASE   (DPKEY_END_NORMAL+1) /* MWKEY code for first HAVi key */
#define DPKEY_HAVI_CODE_FIRST  403			/* HAVi code for first HAVi key */
#define DPKEY_HAVI_CODE_LAST   460			/* HAVi code for last HAVi key */
/* HRcEvent.VK_... code to DPKEY_... code */
#define DPKEY_FROM_HAVI_CODE(h) ((h) + (DPKEY_HAVI_KEY_BASE - DPKEY_HAVI_CODE_FIRST))
/* DPKEY_... code to HRcEvent.VK_... code */
#define DPKEY_TO_HAVI_CODE(m)   ((m) - (DPKEY_HAVI_KEY_BASE - DPKEY_HAVI_CODE_FIRST))
/* Can an DPKEY_... code be converted into a HRcEvent.VK_... code? */
#define DPKEY_IS_HAVI_CODE(m)   (  (unsigned)((m) - DPKEY_HAVI_KEY_BASE) \
               <= (unsigned)(DPKEY_HAVI_CODE_LAST - DPKEY_HAVI_CODE_FIRST) )

#define DPKEY_COLORED_KEY_0         DPKEY_FROM_HAVI_CODE(403)
#define DPKEY_COLORED_KEY_1         DPKEY_FROM_HAVI_CODE(404)
#define DPKEY_COLORED_KEY_2         DPKEY_FROM_HAVI_CODE(405)
#define DPKEY_COLORED_KEY_3         DPKEY_FROM_HAVI_CODE(406)
#define DPKEY_COLORED_KEY_4         DPKEY_FROM_HAVI_CODE(407)
#define DPKEY_COLORED_KEY_5         DPKEY_FROM_HAVI_CODE(408)
#define DPKEY_POWER                 DPKEY_FROM_HAVI_CODE(409)
#define DPKEY_DIMMER                DPKEY_FROM_HAVI_CODE(410)
#define DPKEY_WINK                  DPKEY_FROM_HAVI_CODE(411)
#define DPKEY_REWIND                DPKEY_FROM_HAVI_CODE(412)
#define DPKEY_STOP                  DPKEY_FROM_HAVI_CODE(413)
#define DPKEY_EJECT_TOGGLE          DPKEY_FROM_HAVI_CODE(414)
#define DPKEY_PLAY                  DPKEY_FROM_HAVI_CODE(415)
#define DPKEY_RECORD                DPKEY_FROM_HAVI_CODE(416)
#define DPKEY_FAST_FWD              DPKEY_FROM_HAVI_CODE(417)
#define DPKEY_PLAY_SPEED_UP         DPKEY_FROM_HAVI_CODE(418)
#define DPKEY_PLAY_SPEED_DOWN       DPKEY_FROM_HAVI_CODE(419)
#define DPKEY_PLAY_SPEED_RESET      DPKEY_FROM_HAVI_CODE(420)
#define DPKEY_RECORD_SPEED_NEXT     DPKEY_FROM_HAVI_CODE(421)
#define DPKEY_GO_TO_START           DPKEY_FROM_HAVI_CODE(422)
#define DPKEY_GO_TO_END             DPKEY_FROM_HAVI_CODE(423)
#define DPKEY_TRACK_PREV            DPKEY_FROM_HAVI_CODE(424)
#define DPKEY_TRACK_NEXT            DPKEY_FROM_HAVI_CODE(425)
#define DPKEY_RANDOM_TOGGLE         DPKEY_FROM_HAVI_CODE(426)
#define DPKEY_CHANNEL_UP            DPKEY_FROM_HAVI_CODE(427)
#define DPKEY_CHANNEL_DOWN          DPKEY_FROM_HAVI_CODE(428)
#define DPKEY_STORE_FAVORITE_0      DPKEY_FROM_HAVI_CODE(429)
#define DPKEY_STORE_FAVORITE_1      DPKEY_FROM_HAVI_CODE(430)
#define DPKEY_STORE_FAVORITE_2      DPKEY_FROM_HAVI_CODE(431)
#define DPKEY_STORE_FAVORITE_3      DPKEY_FROM_HAVI_CODE(432)
#define DPKEY_RECALL_FAVORITE_0     DPKEY_FROM_HAVI_CODE(433)
#define DPKEY_RECALL_FAVORITE_1     DPKEY_FROM_HAVI_CODE(434)
#define DPKEY_RECALL_FAVORITE_2     DPKEY_FROM_HAVI_CODE(435)
#define DPKEY_RECALL_FAVORITE_3     DPKEY_FROM_HAVI_CODE(436)
#define DPKEY_CLEAR_FAVORITE_0      DPKEY_FROM_HAVI_CODE(437)
#define DPKEY_CLEAR_FAVORITE_1      DPKEY_FROM_HAVI_CODE(438)
#define DPKEY_CLEAR_FAVORITE_2      DPKEY_FROM_HAVI_CODE(439)
#define DPKEY_CLEAR_FAVORITE_3      DPKEY_FROM_HAVI_CODE(440)
#define DPKEY_SCAN_CHANNELS_TOGGLE  DPKEY_FROM_HAVI_CODE(441)
#define DPKEY_PINP_TOGGLE           DPKEY_FROM_HAVI_CODE(442)
#define DPKEY_SPLIT_SCREEN_TOGGLE   DPKEY_FROM_HAVI_CODE(443)
#define DPKEY_DISPLAY_SWAP          DPKEY_FROM_HAVI_CODE(444)
#define DPKEY_SCREEN_MODE_NEXT      DPKEY_FROM_HAVI_CODE(445)
#define DPKEY_VIDEO_MODE_NEXT       DPKEY_FROM_HAVI_CODE(446)
#define DPKEY_VOLUME_UP             DPKEY_FROM_HAVI_CODE(447)
#define DPKEY_VOLUME_DOWN           DPKEY_FROM_HAVI_CODE(448)
#define DPKEY_MUTE                  DPKEY_FROM_HAVI_CODE(449)
#define DPKEY_SURROUND_MODE_NEXT    DPKEY_FROM_HAVI_CODE(450)
#define DPKEY_BALANCE_RIGHT         DPKEY_FROM_HAVI_CODE(451)
#define DPKEY_BALANCE_LEFT          DPKEY_FROM_HAVI_CODE(452)
#define DPKEY_FADER_FRONT           DPKEY_FROM_HAVI_CODE(453)
#define DPKEY_FADER_REAR            DPKEY_FROM_HAVI_CODE(454)
#define DPKEY_BASS_BOOST_UP         DPKEY_FROM_HAVI_CODE(455)
#define DPKEY_BASS_BOOST_DOWN       DPKEY_FROM_HAVI_CODE(456)
#define DPKEY_INFO                  DPKEY_FROM_HAVI_CODE(457)
#define DPKEY_GUIDE                 DPKEY_FROM_HAVI_CODE(458)
#define DPKEY_TELETEXT              DPKEY_FROM_HAVI_CODE(459)
#define DPKEY_SUBTITLE              DPKEY_FROM_HAVI_CODE(460)

#define DPKEY_LAST                  DPKEY_SUBTITLE

/* Keyboard state modifiers*/
#define DPKMOD_NONE  		0x0000
#define DPKMOD_LSHIFT		0x0001
#define DPKMOD_RSHIFT		0x0002
#define DPKMOD_LCTRL 		0x0040
#define DPKMOD_RCTRL 		0x0080
#define DPKMOD_LALT  		0x0100
#define DPKMOD_RALT  		0x0200
#define DPKMOD_LMETA 		0x0400		/* Windows key*/
#define DPKMOD_RMETA 		0x0800		/* Windows key*/
#define DPKMOD_NUM   		0x1000
#define DPKMOD_CAPS  		0x2000
#define DPKMOD_ALTGR 		0x4000
#define DPKMOD_SCR			0x8000

#define DPKMOD_CTRL	(DPKMOD_LCTRL|DPKMOD_RCTRL)
#define DPKMOD_SHIFT	(DPKMOD_LSHIFT|DPKMOD_RSHIFT)
#define DPKMOD_ALT	(DPKMOD_LALT|DPKMOD_RALT)
#define DPKMOD_META	(DPKMOD_LMETA|DPKMOD_RMETA)

typedef struct {
	int led;
	int led_mode;
} DPKBINFO, *PDPKBINFO;

#define DPKINFO_LED_MASK	(1 << 0)
#define DPKINFO_LED_MODE_MASK	(1 << 1)

/* Keyboard info values */
#define DPKINFO_LED_CAP		(1 << 0)
#define DPKINFO_LED_NUM		(1 << 1)
#define DPKINFO_LED_SCR		(1 << 2)
#define DPKINFO_LED_MODE_ON	(1 << 3)
#define DPKINFO_LED_MODE_OFF (1 << 4)
