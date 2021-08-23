/*
 * Copyright 2012 Jacek Caban for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef _INC_UIAUTOMATIONCOREAPI
#define _INC_UIAUTOMATIONCOREAPI

#ifdef __cplusplus
extern "C" {
#endif

#define UIA_E_ELEMENTNOTENABLED       0x80040200
#define UIA_E_ELEMENTNOTAVAILABLE     0x80040201
#define UIA_E_NOCLICKABLEPOINT        0x80040202
#define UIA_E_PROXYASSEMBLYNOTLOADED  0x80040203
#define UIA_E_NOTSUPPORTED            0x80040204
#define UIA_E_INVALIDOPERATION        0x80131509
#define UIA_E_TIMEOUT                 0x80131505

#define UiaAppendRuntimeId  3
#define UiaRootObjectId     -25

#define UIA_PFIA_DEFAULT       0x00
#define UIA_PFIA_UNWRAP_BRIDGE 0x01

DECLARE_HANDLE(HUIANODE);
DECLARE_HANDLE(HUIAPATTERNOBJECT);
DECLARE_HANDLE(HUIATEXTRANGE);
DECLARE_HANDLE(HUIAEVENT);

/*
 * AutomationIdentifierType_Property GUIDs.
 */
DEFINE_GUID(RuntimeId_Property_GUID,                           0xa39eebfa,0x7fba,0x4c89,0xb4,0xd4,0xb9,0x9e,0x2d,0xe7,0xd1,0x60);
DEFINE_GUID(BoundingRectangle_Property_GUID,                   0x7bbfe8b2,0x3bfc,0x48dd,0xb7,0x29,0xc7,0x94,0xb8,0x46,0xe9,0xa1);
DEFINE_GUID(ProcessId_Property_GUID,                           0x40499998,0x9c31,0x4245,0xa4,0x03,0x87,0x32,0x0e,0x59,0xea,0xf6);
DEFINE_GUID(ControlType_Property_GUID,                         0xca774fea,0x28ac,0x4bc2,0x94,0xca,0xac,0xec,0x6d,0x6c,0x10,0xa3);
DEFINE_GUID(LocalizedControlType_Property_GUID,                0x8763404f,0xa1bd,0x452a,0x89,0xc4,0x3f,0x01,0xd3,0x83,0x38,0x06);
DEFINE_GUID(Name_Property_GUID,                                0xc3a6921b,0x4a99,0x44f1,0xbc,0xa6,0x61,0x18,0x70,0x52,0xc4,0x31);
DEFINE_GUID(AcceleratorKey_Property_GUID,                      0x514865df,0x2557,0x4cb9,0xae,0xed,0x6c,0xed,0x08,0x4c,0xe5,0x2c);
DEFINE_GUID(AccessKey_Property_GUID,                           0x06827b12,0xa7f9,0x4a15,0x91,0x7c,0xff,0xa5,0xad,0x3e,0xb0,0xa7);
DEFINE_GUID(HasKeyboardFocus_Property_GUID,                    0xcf8afd39,0x3f46,0x4800,0x96,0x56,0xb2,0xbf,0x12,0x52,0x99,0x05);
DEFINE_GUID(IsKeyboardFocusable_Property_GUID,                 0xf7b8552a,0x0859,0x4b37,0xb9,0xcb,0x51,0xe7,0x20,0x92,0xf2,0x9f);
DEFINE_GUID(IsEnabled_Property_GUID,                           0x2109427f,0xda60,0x4fed,0xbf,0x1b,0x26,0x4b,0xdc,0xe6,0xeb,0x3a);
DEFINE_GUID(AutomationId_Property_GUID,                        0xc82c0500,0xb60e,0x4310,0xa2,0x67,0x30,0x3c,0x53,0x1f,0x8e,0xe5);
DEFINE_GUID(ClassName_Property_GUID,                           0x157b7215,0x894f,0x4b65,0x84,0xe2,0xaa,0xc0,0xda,0x08,0xb1,0x6b);
DEFINE_GUID(HelpText_Property_GUID,                            0x08555685,0x0977,0x45c7,0xa7,0xa6,0xab,0xaf,0x56,0x84,0x12,0x1a);
DEFINE_GUID(ClickablePoint_Property_GUID,                      0x0196903b,0xb203,0x4818,0xa9,0xf3,0xf0,0x8e,0x67,0x5f,0x23,0x41);
DEFINE_GUID(Culture_Property_GUID,                             0xe2d74f27,0x3d79,0x4dc2,0xb8,0x8b,0x30,0x44,0x96,0x3a,0x8a,0xfb);
DEFINE_GUID(IsControlElement_Property_GUID,                    0x95f35085,0xabcc,0x4afd,0xa5,0xf4,0xdb,0xb4,0x6c,0x23,0x0f,0xdb);
DEFINE_GUID(IsContentElement_Property_GUID,                    0x4bda64a8,0xf5d8,0x480b,0x81,0x55,0xef,0x2e,0x89,0xad,0xb6,0x72);
DEFINE_GUID(LabeledBy_Property_GUID,                           0xe5b8924b,0xfc8a,0x4a35,0x80,0x31,0xcf,0x78,0xac,0x43,0xe5,0x5e);
DEFINE_GUID(IsPassword_Property_GUID,                          0xe8482eb1,0x687c,0x497b,0xbe,0xbc,0x03,0xbe,0x53,0xec,0x14,0x54);
DEFINE_GUID(NewNativeWindowHandle_Property_GUID,               0x5196b33b,0x380a,0x4982,0x95,0xe1,0x91,0xf3,0xef,0x60,0xe0,0x24);
DEFINE_GUID(ItemType_Property_GUID,                            0xcdda434d,0x6222,0x413b,0xa6,0x8a,0x32,0x5d,0xd1,0xd4,0x0f,0x39);
DEFINE_GUID(IsOffscreen_Property_GUID,                         0x03c3d160,0xdb79,0x42db,0xa2,0xef,0x1c,0x23,0x1e,0xed,0xe5,0x07);
DEFINE_GUID(Orientation_Property_GUID,                         0xa01eee62,0x3884,0x4415,0x88,0x7e,0x67,0x8e,0xc2,0x1e,0x39,0xba);
DEFINE_GUID(FrameworkId_Property_GUID,                         0xdbfd9900,0x7e1a,0x4f58,0xb6,0x1b,0x70,0x63,0x12,0x0f,0x77,0x3b);
DEFINE_GUID(IsRequiredForForm_Property_GUID,                   0x4f5f43cf,0x59fb,0x4bde,0xa2,0x70,0x60,0x2e,0x5e,0x11,0x41,0xe9);
DEFINE_GUID(ItemStatus_Property_GUID,                          0x51de0321,0x3973,0x43e7,0x89,0x13,0x0b,0x08,0xe8,0x13,0xc3,0x7f);
DEFINE_GUID(IsDockPatternAvailable_Property_GUID,              0x2600a4c4,0x2ff8,0x4c96,0xae,0x31,0x8f,0xe6,0x19,0xa1,0x3c,0x6c);
DEFINE_GUID(IsExpandCollapsePatternAvailable_Property_GUID,    0x929d3806,0x5287,0x4725,0xaa,0x16,0x22,0x2a,0xfc,0x63,0xd5,0x95);
DEFINE_GUID(IsGridItemPatternAvailable_Property_GUID,          0x5a43e524,0xf9a2,0x4b12,0x84,0xc8,0xb4,0x8a,0x3e,0xfe,0xdd,0x34);
DEFINE_GUID(IsGridPatternAvailable_Property_GUID,              0x5622c26c,0xf0ef,0x4f3b,0x97,0xcb,0x71,0x4c,0x08,0x68,0x58,0x8b);
DEFINE_GUID(IsInvokePatternAvailable_Property_GUID,            0x4e725738,0x8364,0x4679,0xaa,0x6c,0xf3,0xf4,0x19,0x31,0xf7,0x50);
DEFINE_GUID(IsMultipleViewPatternAvailable_Property_GUID,      0xff0a31eb,0x8e25,0x469d,0x8d,0x6e,0xe7,0x71,0xa2,0x7c,0x1b,0x90);
DEFINE_GUID(IsRangeValuePatternAvailable_Property_GUID,        0xfda4244a,0xeb4d,0x43ff,0xb5,0xad,0xed,0x36,0xd3,0x73,0xec,0x4c);
DEFINE_GUID(IsScrollPatternAvailable_Property_GUID,            0x3ebb7b4a,0x828a,0x4b57,0x9d,0x22,0x2f,0xea,0x16,0x32,0xed,0x0d);
DEFINE_GUID(IsScrollItemPatternAvailable_Property_GUID,        0x1cad1a05,0x0927,0x4b76,0x97,0xe1,0x0f,0xcd,0xb2,0x09,0xb9,0x8a);
DEFINE_GUID(IsSelectionItemPatternAvailable_Property_GUID,     0x8becd62d,0x0bc3,0x4109,0xbe,0xe2,0x8e,0x67,0x15,0x29,0x0e,0x68);
DEFINE_GUID(IsSelectionPatternAvailable_Property_GUID,         0xf588acbe,0xc769,0x4838,0x9a,0x60,0x26,0x86,0xdc,0x11,0x88,0xc4);
DEFINE_GUID(IsTablePatternAvailable_Property_GUID,             0xcb83575f,0x45c2,0x4048,0x9c,0x76,0x15,0x97,0x15,0xa1,0x39,0xdf);
DEFINE_GUID(IsTableItemPatternAvailable_Property_GUID,         0xeb36b40d,0x8ea4,0x489b,0xa0,0x13,0xe6,0x0d,0x59,0x51,0xfe,0x34);
DEFINE_GUID(IsTextPatternAvailable_Property_GUID,              0xfbe2d69d,0xaff6,0x4a45,0x82,0xe2,0xfc,0x92,0xa8,0x2f,0x59,0x17);
DEFINE_GUID(IsTogglePatternAvailable_Property_GUID,            0x78686d53,0xfcd0,0x4b83,0x9b,0x78,0x58,0x32,0xce,0x63,0xbb,0x5b);
DEFINE_GUID(IsTransformPatternAvailable_Property_GUID,         0xa7f78804,0xd68b,0x4077,0xa5,0xc6,0x7a,0x5e,0xa1,0xac,0x31,0xc5);
DEFINE_GUID(IsValuePatternAvailable_Property_GUID,             0x0b5020a7,0x2119,0x473b,0xbe,0x37,0x5c,0xeb,0x98,0xbb,0xfb,0x22);
DEFINE_GUID(IsWindowPatternAvailable_Property_GUID,            0xe7a57bb1,0x5888,0x4155,0x98,0xdc,0xb4,0x22,0xfd,0x57,0xf2,0xbc);
DEFINE_GUID(Value_Value_Property_GUID,                         0xe95f5e64,0x269f,0x4a85,0xba,0x99,0x40,0x92,0xc3,0xea,0x29,0x86);
DEFINE_GUID(Value_IsReadOnly_Property_GUID,                    0xeb090f30,0xe24c,0x4799,0xa7,0x05,0x0d,0x24,0x7b,0xc0,0x37,0xf8);
DEFINE_GUID(RangeValue_Value_Property_GUID,                    0x131f5d98,0xc50c,0x489d,0xab,0xe5,0xae,0x22,0x08,0x98,0xc5,0xf7);
DEFINE_GUID(RangeValue_IsReadOnly_Property_GUID,               0x25fa1055,0xdebf,0x4373,0xa7,0x9e,0x1f,0x1a,0x19,0x08,0xd3,0xc4);
DEFINE_GUID(RangeValue_Minimum_Property_GUID,                  0x78cbd3b2,0x684d,0x4860,0xaf,0x93,0xd1,0xf9,0x5c,0xb0,0x22,0xfd);
DEFINE_GUID(RangeValue_Maximum_Property_GUID,                  0x19319914,0xf979,0x4b35,0xa1,0xa6,0xd3,0x7e,0x05,0x43,0x34,0x73);
DEFINE_GUID(RangeValue_LargeChange_Property_GUID,              0xa1f96325,0x3a3d,0x4b44,0x8e,0x1f,0x4a,0x46,0xd9,0x84,0x40,0x19);
DEFINE_GUID(RangeValue_SmallChange_Property_GUID,              0x81c2c457,0x3941,0x4107,0x99,0x75,0x13,0x97,0x60,0xf7,0xc0,0x72);
DEFINE_GUID(Scroll_HorizontalScrollPercent_Property_GUID,      0xc7c13c0e,0xeb21,0x47ff,0xac,0xc4,0xb5,0xa3,0x35,0x0f,0x51,0x91);
DEFINE_GUID(Scroll_HorizontalViewSize_Property_GUID,           0x70c2e5d4,0xfcb0,0x4713,0xa9,0xaa,0xaf,0x92,0xff,0x79,0xe4,0xcd);
DEFINE_GUID(Scroll_VerticalScrollPercent_Property_GUID,        0x6c8d7099,0xb2a8,0x4948,0xbf,0xf7,0x3c,0xf9,0x05,0x8b,0xfe,0xfb);
DEFINE_GUID(Scroll_VerticalViewSize_Property_GUID,             0xde6a2e22,0xd8c7,0x40c5,0x83,0xba,0xe5,0xf6,0x81,0xd5,0x31,0x08);
DEFINE_GUID(Scroll_HorizontallyScrollable_Property_GUID,       0x8b925147,0x28cd,0x49ae,0xbd,0x63,0xf4,0x41,0x18,0xd2,0xe7,0x19);
DEFINE_GUID(Scroll_VerticallyScrollable_Property_GUID,         0x89164798,0x0068,0x4315,0xb8,0x9a,0x1e,0x7c,0xfb,0xbc,0x3d,0xfc);
DEFINE_GUID(Selection_Selection_Property_GUID,                 0xaa6dc2a2,0x0e2b,0x4d38,0x96,0xd5,0x34,0xe4,0x70,0xb8,0x18,0x53);
DEFINE_GUID(Selection_CanSelectMultiple_Property_GUID,         0x49d73da5,0xc883,0x4500,0x88,0x3d,0x8f,0xcf,0x8d,0xaf,0x6c,0xbe);
DEFINE_GUID(Selection_IsSelectionRequired_Property_GUID,       0xb1ae4422,0x63fe,0x44e7,0xa5,0xa5,0xa7,0x38,0xc8,0x29,0xb1,0x9a);
DEFINE_GUID(Grid_RowCount_Property_GUID,                       0x2a9505bf,0xc2eb,0x4fb6,0xb3,0x56,0x82,0x45,0xae,0x53,0x70,0x3e);
DEFINE_GUID(Grid_ColumnCount_Property_GUID,                    0xfe96f375,0x44aa,0x4536,0xac,0x7a,0x2a,0x75,0xd7,0x1a,0x3e,0xfc);
DEFINE_GUID(GridItem_Row_Property_GUID,                        0x6223972a,0xc945,0x4563,0x93,0x29,0xfd,0xc9,0x74,0xaf,0x25,0x53);
DEFINE_GUID(GridItem_Column_Property_GUID,                     0xc774c15c,0x62c0,0x4519,0x8b,0xdc,0x47,0xbe,0x57,0x3c,0x8a,0xd5);
DEFINE_GUID(GridItem_RowSpan_Property_GUID,                    0x4582291c,0x466b,0x4e93,0x8e,0x83,0x3d,0x17,0x15,0xec,0x0c,0x5e);
DEFINE_GUID(GridItem_ColumnSpan_Property_GUID,                 0x583ea3f5,0x86d0,0x4b08,0xa6,0xec,0x2c,0x54,0x63,0xff,0xc1,0x09);
DEFINE_GUID(GridItem_Parent_Property_GUID,                     0x9d912252,0xb97f,0x4ecc,0x85,0x10,0xea,0x0e,0x33,0x42,0x7c,0x72);
DEFINE_GUID(Dock_DockPosition_Property_GUID,                   0x6d67f02e,0xc0b0,0x4b10,0xb5,0xb9,0x18,0xd6,0xec,0xf9,0x87,0x60);
DEFINE_GUID(ExpandCollapse_ExpandCollapseState_Property_GUID,  0x275a4c48,0x85a7,0x4f69,0xab,0xa0,0xaf,0x15,0x76,0x10,0x00,0x2b);
DEFINE_GUID(MultipleView_CurrentView_Property_GUID,            0x7a81a67a,0xb94f,0x4875,0x91,0x8b,0x65,0xc8,0xd2,0xf9,0x98,0xe5);
DEFINE_GUID(MultipleView_SupportedViews_Property_GUID,         0x8d5db9fd,0xce3c,0x4ae7,0xb7,0x88,0x40,0x0a,0x3c,0x64,0x55,0x47);
DEFINE_GUID(Window_CanMaximize_Property_GUID,                  0x64fff53f,0x635d,0x41c1,0x95,0x0c,0xcb,0x5a,0xdf,0xbe,0x28,0xe3);
DEFINE_GUID(Window_CanMinimize_Property_GUID,                  0xb73b4625,0x5988,0x4b97,0xb4,0xc2,0xa6,0xfe,0x6e,0x78,0xc8,0xc6);
DEFINE_GUID(Window_WindowVisualState_Property_GUID,            0x4ab7905f,0xe860,0x453e,0xa3,0x0a,0xf6,0x43,0x1e,0x5d,0xaa,0xd5);
DEFINE_GUID(Window_WindowInteractionState_Property_GUID,       0x4fed26a4,0x0455,0x4fa2,0xb2,0x1c,0xc4,0xda,0x2d,0xb1,0xff,0x9c);
DEFINE_GUID(Window_IsModal_Property_GUID,                      0xff4e6892,0x37b9,0x4fca,0x85,0x32,0xff,0xe6,0x74,0xec,0xfe,0xed);
DEFINE_GUID(Window_IsTopmost_Property_GUID,                    0xef7d85d3,0x0937,0x4962,0x92,0x41,0xb6,0x23,0x45,0xf2,0x40,0x41);
DEFINE_GUID(SelectionItem_IsSelected_Property_GUID,            0xf122835f,0xcd5f,0x43df,0xb7,0x9d,0x4b,0x84,0x9e,0x9e,0x60,0x20);
DEFINE_GUID(SelectionItem_SelectionContainer_Property_GUID,    0xa4365b6e,0x9c1e,0x4b63,0x8b,0x53,0xc2,0x42,0x1d,0xd1,0xe8,0xfb);
DEFINE_GUID(Table_RowHeaders_Property_GUID,                    0xd9e35b87,0x6eb8,0x4562,0xaa,0xc6,0xa8,0xa9,0x07,0x52,0x36,0xa8);
DEFINE_GUID(Table_ColumnHeaders_Property_GUID,                 0xaff1d72b,0x968d,0x42b1,0xb4,0x59,0x15,0x0b,0x29,0x9d,0xa6,0x64);
DEFINE_GUID(Table_RowOrColumnMajor_Property_GUID,              0x83be75c3,0x29fe,0x4a30,0x85,0xe1,0x2a,0x62,0x77,0xfd,0x10,0x6e);
DEFINE_GUID(TableItem_RowHeaderItems_Property_GUID,            0xb3f853a0,0x0574,0x4cd8,0xbc,0xd7,0xed,0x59,0x23,0x57,0x2d,0x97);
DEFINE_GUID(TableItem_ColumnHeaderItems_Property_GUID,         0x967a56a3,0x74b6,0x431e,0x8d,0xe6,0x99,0xc4,0x11,0x03,0x1c,0x58);
DEFINE_GUID(Toggle_ToggleState_Property_GUID,                  0xb23cdc52,0x22c2,0x4c6c,0x9d,0xed,0xf5,0xc4,0x22,0x47,0x9e,0xde);
DEFINE_GUID(Transform_CanMove_Property_GUID,                   0x1b75824d,0x208b,0x4fdf,0xbc,0xcd,0xf1,0xf4,0xe5,0x74,0x1f,0x4f);
DEFINE_GUID(Transform_CanResize_Property_GUID,                 0xbb98dca5,0x4c1a,0x41d4,0xa4,0xf6,0xeb,0xc1,0x28,0x64,0x41,0x80);
DEFINE_GUID(Transform_CanRotate_Property_GUID,                 0x10079b48,0x3849,0x476f,0xac,0x96,0x44,0xa9,0x5c,0x84,0x40,0xd9);
DEFINE_GUID(IsLegacyIAccessiblePatternAvailable_Property_GUID, 0xd8ebd0c7,0x929a,0x4ee7,0x8d,0x3a,0xd3,0xd9,0x44,0x13,0x02,0x7b);
DEFINE_GUID(LegacyIAccessible_ChildId_Property_GUID,           0x9a191b5d,0x9ef2,0x4787,0xa4,0x59,0xdc,0xde,0x88,0x5d,0xd4,0xe8);
DEFINE_GUID(LegacyIAccessible_Name_Property_GUID,              0xcaeb063d,0x40ae,0x4869,0xaa,0x5a,0x1b,0x8e,0x5d,0x66,0x67,0x39);
DEFINE_GUID(LegacyIAccessible_Value_Property_GUID,             0xb5c5b0b6,0x8217,0x4a77,0x97,0xa5,0x19,0x0a,0x85,0xed,0x01,0x56);
DEFINE_GUID(LegacyIAccessible_Description_Property_GUID,       0x46448418,0x7d70,0x4ea9,0x9d,0x27,0xb7,0xe7,0x75,0xcf,0x2a,0xd7);
DEFINE_GUID(LegacyIAccessible_Role_Property_GUID,              0x6856e59f,0xcbaf,0x4e31,0x93,0xe8,0xbc,0xbf,0x6f,0x7e,0x49,0x1c);
DEFINE_GUID(LegacyIAccessible_State_Property_GUID,             0xdf985854,0x2281,0x4340,0xab,0x9c,0xc6,0x0e,0x2c,0x58,0x03,0xf6);
DEFINE_GUID(LegacyIAccessible_Help_Property_GUID,              0x94402352,0x161c,0x4b77,0xa9,0x8d,0xa8,0x72,0xcc,0x33,0x94,0x7a);
DEFINE_GUID(LegacyIAccessible_KeyboardShortcut_Property_GUID,  0x8f6909ac,0x00b8,0x4259,0xa4,0x1c,0x96,0x62,0x66,0xd4,0x3a,0x8a);
DEFINE_GUID(LegacyIAccessible_Selection_Property_GUID,         0x8aa8b1e0,0x0891,0x40cc,0x8b,0x06,0x90,0xd7,0xd4,0x16,0x62,0x19);
DEFINE_GUID(LegacyIAccessible_DefaultAction_Property_GUID,     0x3b331729,0xeaad,0x4502,0xb8,0x5f,0x92,0x61,0x56,0x22,0x91,0x3c);
DEFINE_GUID(AriaRole_Property_GUID,                            0xdd207b95,0xbe4a,0x4e0d,0xb7,0x27,0x63,0xac,0xe9,0x4b,0x69,0x16);
DEFINE_GUID(AriaProperties_Property_GUID,                      0x4213678c,0xe025,0x4922,0xbe,0xb5,0xe4,0x3b,0xa0,0x8e,0x62,0x21);
DEFINE_GUID(IsDataValidForForm_Property_GUID,                  0x445ac684,0xc3fc,0x4dd9,0xac,0xf8,0x84,0x5a,0x57,0x92,0x96,0xba);
DEFINE_GUID(ControllerFor_Property_GUID,                       0x51124c8a,0xa5d2,0x4f13,0x9b,0xe6,0x7f,0xa8,0xba,0x9d,0x3a,0x90);
DEFINE_GUID(DescribedBy_Property_GUID,                         0x7c5865b8,0x9992,0x40fd,0x8d,0xb0,0x6b,0xf1,0xd3,0x17,0xf9,0x98);
DEFINE_GUID(FlowsTo_Property_GUID,                             0xe4f33d20,0x559a,0x47fb,0xa8,0x30,0xf9,0xcb,0x4f,0xf1,0xa7,0x0a);
DEFINE_GUID(ProviderDescription_Property_GUID,                 0xdca5708a,0xc16b,0x4cd9,0xb8,0x89,0xbe,0xb1,0x6a,0x80,0x49,0x04);
DEFINE_GUID(IsItemContainerPatternAvailable_Property_GUID,     0x624b5ca7,0xfe40,0x4957,0xa0,0x19,0x20,0xc4,0xcf,0x11,0x92,0x0f);
DEFINE_GUID(IsVirtualizedItemPatternAvailable_Property_GUID,   0x302cb151,0x2ac8,0x45d6,0x97,0x7b,0xd2,0xb3,0xa5,0xa5,0x3f,0x20);
DEFINE_GUID(IsSynchronizedInputPatternAvailable_Property_GUID, 0x75d69cc5,0xd2bf,0x4943,0x87,0x6e,0xb4,0x5b,0x62,0xa6,0xcc,0x66);
DEFINE_GUID(OptimizeForVisualContent_Property_GUID,            0x6a852250,0xc75a,0x4e5d,0xb8,0x58,0xe3,0x81,0xb0,0xf7,0x88,0x61);
DEFINE_GUID(IsObjectModelPatternAvailable_Property_GUID,       0x6b21d89b,0x2841,0x412f,0x8e,0xf2,0x15,0xca,0x95,0x23,0x18,0xba);
DEFINE_GUID(Annotation_AnnotationTypeId_Property_GUID,         0x20ae484f,0x69ef,0x4c48,0x8f,0x5b,0xc4,0x93,0x8b,0x20,0x6a,0xc7);
DEFINE_GUID(Annotation_AnnotationTypeName_Property_GUID,       0x9b818892,0x5ac9,0x4af9,0xaa,0x96,0xf5,0x8a,0x77,0xb0,0x58,0xe3);
DEFINE_GUID(Annotation_Author_Property_GUID,                   0x7a528462,0x9c5c,0x4a03,0xa9,0x74,0x8b,0x30,0x7a,0x99,0x37,0xf2);
DEFINE_GUID(Annotation_DateTime_Property_GUID,                 0x99b5ca5d,0x1acf,0x414b,0xa4,0xd0,0x6b,0x35,0x0b,0x04,0x75,0x78);
DEFINE_GUID(Annotation_Target_Property_GUID,                   0xb71b302d,0x2104,0x44ad,0x9c,0x5c,0x09,0x2b,0x49,0x07,0xd7,0x0f);
DEFINE_GUID(IsAnnotationPatternAvailable_Property_GUID,        0x0b5b3238,0x6d5c,0x41b6,0xbc,0xc4,0x5e,0x80,0x7f,0x65,0x51,0xc4);
DEFINE_GUID(IsTextPattern2Available_Property_GUID,             0x41cf921d,0xe3f1,0x4b22,0x9c,0x81,0xe1,0xc3,0xed,0x33,0x1c,0x22);
DEFINE_GUID(Styles_StyleId_Property_GUID,                      0xda82852f,0x3817,0x4233,0x82,0xaf,0x02,0x27,0x9e,0x72,0xcc,0x77);
DEFINE_GUID(Styles_StyleName_Property_GUID,                    0x1c12b035,0x05d1,0x4f55,0x9e,0x8e,0x14,0x89,0xf3,0xff,0x55,0x0d);
DEFINE_GUID(Styles_FillColor_Property_GUID,                    0x63eff97a,0xa1c5,0x4b1d,0x84,0xeb,0xb7,0x65,0xf2,0xed,0xd6,0x32);
DEFINE_GUID(Styles_FillPatternStyle_Property_GUID,             0x81cf651f,0x482b,0x4451,0xa3,0x0a,0xe1,0x54,0x5e,0x55,0x4f,0xb8);
DEFINE_GUID(Styles_Shape_Property_GUID,                        0xc71a23f8,0x778c,0x400d,0x84,0x58,0x3b,0x54,0x3e,0x52,0x69,0x84);
DEFINE_GUID(Styles_FillPatternColor_Property_GUID,             0x939a59fe,0x8fbd,0x4e75,0xa2,0x71,0xac,0x45,0x95,0x19,0x51,0x63);
DEFINE_GUID(Styles_ExtendedProperties_Property_GUID,           0xf451cda0,0xba0a,0x4681,0xb0,0xb0,0x0d,0xbd,0xb5,0x3e,0x58,0xf3);
DEFINE_GUID(IsStylesPatternAvailable_Property_GUID,            0x27f353d3,0x459c,0x4b59,0xa4,0x90,0x50,0x61,0x1d,0xac,0xaf,0xb5);
DEFINE_GUID(IsSpreadsheetPatternAvailable_Property_GUID,       0x6ff43732,0xe4b4,0x4555,0x97,0xbc,0xec,0xdb,0xbc,0x4d,0x18,0x88);
DEFINE_GUID(SpreadsheetItem_Formula_Property_GUID,             0xe602e47d,0x1b47,0x4bea,0x87,0xcf,0x3b,0x0b,0x0b,0x5c,0x15,0xb6);
DEFINE_GUID(SpreadsheetItem_AnnotationObjects_Property_GUID,   0xa3194c38,0xc9bc,0x4604,0x93,0x96,0xae,0x3f,0x9f,0x45,0x7f,0x7b);
DEFINE_GUID(SpreadsheetItem_AnnotationTypes_Property_GUID,     0xc70c51d0,0xd602,0x4b45,0xaf,0xbc,0xb4,0x71,0x2b,0x96,0xd7,0x2b);
DEFINE_GUID(IsSpreadsheetItemPatternAvailable_Property_GUID,   0x9fe79b2a,0x2f94,0x43fd,0x99,0x6b,0x54,0x9e,0x31,0x6f,0x4a,0xcd);
DEFINE_GUID(Transform2_CanZoom_Property_GUID,                  0xf357e890,0xa756,0x4359,0x9c,0xa6,0x86,0x70,0x2b,0xf8,0xf3,0x81);
DEFINE_GUID(IsTransformPattern2Available_Property_GUID,        0x25980b4b,0xbe04,0x4710,0xab,0x4a,0xfd,0xa3,0x1d,0xbd,0x28,0x95);
DEFINE_GUID(LiveSetting_Property_GUID,                         0xc12bcd8e,0x2a8e,0x4950,0x8a,0xe7,0x36,0x25,0x11,0x1d,0x58,0xeb);
DEFINE_GUID(IsTextChildPatternAvailable_Property_GUID,         0x559e65df,0x30ff,0x43b5,0xb5,0xed,0x5b,0x28,0x3b,0x80,0xc7,0xe9);
DEFINE_GUID(IsDragPatternAvailable_Property_GUID,              0xe997a7b7,0x1d39,0x4ca7,0xbe,0x0f,0x27,0x7f,0xcf,0x56,0x05,0xcc);
DEFINE_GUID(Drag_IsGrabbed_Property_GUID,                      0x45f206f3,0x75cc,0x4cca,0xa9,0xb9,0xfc,0xdf,0xb9,0x82,0xd8,0xa2);
DEFINE_GUID(Drag_DropEffect_Property_GUID,                     0x646f2779,0x48d3,0x4b23,0x89,0x02,0x4b,0xf1,0x00,0x00,0x5d,0xf3);
DEFINE_GUID(Drag_DropEffects_Property_GUID,                    0xf5d61156,0x7ce6,0x49be,0xa8,0x36,0x92,0x69,0xdc,0xec,0x92,0x0f);
DEFINE_GUID(IsDropTargetPatternAvailable_Property_GUID,        0x0686b62e,0x8e19,0x4aaf,0x87,0x3d,0x38,0x4f,0x6d,0x3b,0x92,0xbe);
DEFINE_GUID(DropTarget_DropTargetEffect_Property_GUID,         0x8bb75975,0xa0ca,0x4981,0xb8,0x18,0x87,0xfc,0x66,0xe9,0x50,0x9d);
DEFINE_GUID(DropTarget_DropTargetEffects_Property_GUID,        0xbc1dd4ed,0xcb89,0x45f1,0xa5,0x92,0xe0,0x3b,0x08,0xae,0x79,0x0f);
DEFINE_GUID(Drag_GrabbedItems_Property_GUID,                   0x77c1562c,0x7b86,0x4b21,0x9e,0xd7,0x3c,0xef,0xda,0x6f,0x4c,0x43);
DEFINE_GUID(Transform2_ZoomLevel_Property_GUID,                0xeee29f1a,0xf4a2,0x4b5b,0xac,0x65,0x95,0xcf,0x93,0x28,0x33,0x87);
DEFINE_GUID(Transform2_ZoomMinimum_Property_GUID,              0x742ccc16,0x4ad1,0x4e07,0x96,0xfe,0xb1,0x22,0xc6,0xe6,0xb2,0x2b);
DEFINE_GUID(Transform2_ZoomMaximum_Property_GUID,              0x42ab6b77,0xceb0,0x4eca,0xb8,0x2a,0x6c,0xfa,0x5f,0xa1,0xfc,0x08);
DEFINE_GUID(FlowsFrom_Property_GUID,                           0x05c6844f,0x19de,0x48f8,0x95,0xfa,0x88,0x0d,0x5b,0x0f,0xd6,0x15);
DEFINE_GUID(IsTextEditPatternAvailable_Property_GUID,          0x7843425c,0x8b32,0x484c,0x9a,0xb5,0xe3,0x20,0x05,0x71,0xff,0xda);
DEFINE_GUID(IsPeripheral_Property_GUID,                        0xda758276,0x7ed5,0x49d4,0x8e,0x68,0xec,0xc9,0xa2,0xd3,0x00,0xdd);
DEFINE_GUID(IsCustomNavigationPatternAvailable_Property_GUID,  0x8f8e80d4,0x2351,0x48e0,0x87,0x4a,0x54,0xaa,0x73,0x13,0x88,0x9a);
DEFINE_GUID(PositionInSet_Property_GUID,                       0x33d1dc54,0x641e,0x4d76,0xa6,0xb1,0x13,0xf3,0x41,0xc1,0xf8,0x96);
DEFINE_GUID(SizeOfSet_Property_GUID,                           0x1600d33c,0x3b9f,0x4369,0x94,0x31,0xaa,0x29,0x3f,0x34,0x4c,0xf1);
DEFINE_GUID(Level_Property_GUID,                               0x242ac529,0xcd36,0x400f,0xaa,0xd9,0x78,0x76,0xef,0x3a,0xf6,0x27);
DEFINE_GUID(AnnotationTypes_Property_GUID,                     0x64b71f76,0x53c4,0x4696,0xa2,0x19,0x20,0xe9,0x40,0xc9,0xa1,0x76);
DEFINE_GUID(AnnotationObjects_Property_GUID,                   0x310910c8,0x7c6e,0x4f20,0xbe,0xcd,0x4a,0xaf,0x6d,0x19,0x11,0x56);
DEFINE_GUID(LandmarkType_Property_GUID,                        0x454045f2,0x6f61,0x49f7,0xa4,0xf8,0xb5,0xf0,0xcf,0x82,0xda,0x1e);
DEFINE_GUID(LocalizedLandmarkType_Property_GUID,               0x7ac81980,0xeafb,0x4fb2,0xbf,0x91,0xf4,0x85,0xbe,0xf5,0xe8,0xe1);
DEFINE_GUID(FullDescription_Property_GUID,                     0x0d4450ff,0x6aef,0x4f33,0x95,0xdd,0x7b,0xef,0xa7,0x2a,0x43,0x91);
DEFINE_GUID(FillColor_Property_GUID ,                          0x6e0ec4d0,0xe2a8,0x4a56,0x9d,0xe7,0x95,0x33,0x89,0x93,0x3b,0x39);
DEFINE_GUID(OutlineColor_Property_GUID,                        0xc395d6c0,0x4b55,0x4762,0xa0,0x73,0xfd,0x30,0x3a,0x63,0x4f,0x52);
DEFINE_GUID(FillType_Property_GUID,                            0xc6fc74e4,0x8cb9,0x429c,0xa9,0xe1,0x9b,0xc4,0xac,0x37,0x2b,0x62);
DEFINE_GUID(VisualEffects_Property_GUID,                       0xe61a8565,0xaad9,0x46d7,0x9e,0x70,0x4e,0x8a,0x84,0x20,0xd4,0x20);
DEFINE_GUID(OutlineThickness_Property_GUID,                    0x13e67cc7,0xdac2,0x4888,0xbd,0xd3,0x37,0x5c,0x62,0xfa,0x96,0x18);
DEFINE_GUID(CenterPoint_Property_GUID,                         0x0cb00c08,0x540c,0x4edb,0x94,0x45,0x26,0x35,0x9e,0xa6,0x97,0x85);
DEFINE_GUID(Rotation_Property_GUID,                            0x767cdc7d,0xaec0,0x4110,0xad,0x32,0x30,0xed,0xd4,0x03,0x49,0x2e);
DEFINE_GUID(Size_Property_GUID,                                0x2b5f761d,0xf885,0x4404,0x97,0x3f,0x9b,0x1d,0x98,0xe3,0x6d,0x8f);
DEFINE_GUID(IsSelectionPattern2Available_Property_GUID,        0x490806fb,0x6e89,0x4a47,0x83,0x19,0xd2,0x66,0xe5,0x11,0xf0,0x21);
DEFINE_GUID(Selection2_FirstSelectedItem_Property_GUID,        0xcc24ea67,0x369c,0x4e55,0x9f,0xf7,0x38,0xda,0x69,0x54,0x0c,0x29);
DEFINE_GUID(Selection2_LastSelectedItem_Property_GUID,         0xcf7bda90,0x2d83,0x49f8,0x86,0x0c,0x9c,0xe3,0x94,0xcf,0x89,0xb4);
DEFINE_GUID(Selection2_CurrentSelectedItem_Property_GUID,      0x34257c26,0x83b5,0x41a6,0x93,0x9c,0xae,0x84,0x1c,0x13,0x62,0x36);
DEFINE_GUID(Selection2_ItemCount_Property_GUID,                0xbb49eb9f,0x456d,0x4048,0xb5,0x91,0x9c,0x20,0x26,0xb8,0x46,0x36);
DEFINE_GUID(HeadingLevel_Property_GUID,                        0x29084272,0xaaaf,0x4a30,0x87,0x96,0x3c,0x12,0xf6,0x2b,0x6b,0xbb);
DEFINE_GUID(IsDialog_Property_GUID,                            0x9d0dfb9b,0x8436,0x4501,0xbb,0xbb,0xe5,0x34,0xa4,0xfb,0x3b,0x3f);

/*
 * AutomationIdentifierType_Event GUIDs.
 */
DEFINE_GUID(ToolTipOpened_Event_GUID,                                  0x3f4b97ff,0x2edc,0x451d,0xbc,0xa4,0x95,0xa3,0x18,0x8d,0x5b,0x03);
DEFINE_GUID(ToolTipClosed_Event_GUID,                                  0x276d71ef,0x24a9,0x49b6,0x8e,0x97,0xda,0x98,0xb4,0x01,0xbb,0xcd);
DEFINE_GUID(StructureChanged_Event_GUID,                               0x59977961,0x3edd,0x4b11,0xb1,0x3b,0x67,0x6b,0x2a,0x2a,0x6c,0xa9);
DEFINE_GUID(MenuOpened_Event_GUID,                                     0xebe2e945,0x66ca,0x4ed1,0x9f,0xf8,0x2a,0xd7,0xdf,0x0a,0x1b,0x08);
DEFINE_GUID(AutomationPropertyChanged_Event_GUID,                      0x2527fba1,0x8d7a,0x4630,0xa4,0xcc,0xe6,0x63,0x15,0x94,0x2f,0x52);
DEFINE_GUID(AutomationFocusChanged_Event_GUID,                         0xb68a1f17,0xf60d,0x41a7,0xa3,0xcc,0xb0,0x52,0x92,0x15,0x5f,0xe0);
DEFINE_GUID(AsyncContentLoaded_Event_GUID,                             0x5fdee11c,0xd2fa,0x4fb9,0x90,0x4e,0x5c,0xbe,0xe8,0x94,0xd5,0xef);
DEFINE_GUID(MenuClosed_Event_GUID,                                     0x3cf1266e,0x1582,0x4041,0xac,0xd7,0x88,0xa3,0x5a,0x96,0x52,0x97);
DEFINE_GUID(LayoutInvalidated_Event_GUID,                              0xed7d6544,0xa6bd,0x4595,0x9b,0xae,0x3d,0x28,0x94,0x6c,0xc7,0x15);
DEFINE_GUID(Invoke_Invoked_Event_GUID,                                 0xdfd699f0,0xc915,0x49dd,0xb4,0x22,0xdd,0xe7,0x85,0xc3,0xd2,0x4b);
DEFINE_GUID(SelectionItem_ElementAddedToSelectionEvent_Event_GUID,     0x3c822dd1,0xc407,0x4dba,0x91,0xdd,0x79,0xd4,0xae,0xd0,0xae,0xc6);
DEFINE_GUID(SelectionItem_ElementRemovedFromSelectionEvent_Event_GUID, 0x097fa8a9,0x7079,0x41af,0x8b,0x9c,0x09,0x34,0xd8,0x30,0x5e,0x5c);
DEFINE_GUID(SelectionItem_ElementSelectedEvent_Event_GUID,             0xb9c7dbfb,0x4ebe,0x4532,0xaa,0xf4,0x00,0x8c,0xf6,0x47,0x23,0x3c);
DEFINE_GUID(Selection_InvalidatedEvent_Event_GUID,                     0xcac14904,0x16b4,0x4b53,0x8e,0x47,0x4c,0xb1,0xdf,0x26,0x7b,0xb7);
DEFINE_GUID(Text_TextSelectionChangedEvent_Event_GUID,                 0x918edaa1,0x71b3,0x49ae,0x97,0x41,0x79,0xbe,0xb8,0xd3,0x58,0xf3);
DEFINE_GUID(Text_TextChangedEvent_Event_GUID,                          0x4a342082,0xf483,0x48c4,0xac,0x11,0xa8,0x4b,0x43,0x5e,0x2a,0x84);
DEFINE_GUID(Window_WindowOpened_Event_GUID,                            0xd3e81d06,0xde45,0x4f2f,0x96,0x33,0xde,0x9e,0x02,0xfb,0x65,0xaf);
DEFINE_GUID(Window_WindowClosed_Event_GUID,                            0xedf141f8,0xfa67,0x4e22,0xbb,0xf7,0x94,0x4e,0x05,0x73,0x5e,0xe2);
DEFINE_GUID(MenuModeStart_Event_GUID,                                  0x18d7c631,0x166a,0x4ac9,0xae,0x3b,0xef,0x4b,0x54,0x20,0xe6,0x81);
DEFINE_GUID(MenuModeEnd_Event_GUID,                                    0x9ecd4c9f,0x80dd,0x47b8,0x82,0x67,0x5a,0xec,0x06,0xbb,0x2c,0xff);
DEFINE_GUID(InputReachedTarget_Event_GUID,                             0x93ed549a,0x0549,0x40f0,0xbe,0xdb,0x28,0xe4,0x4f,0x7d,0xe2,0xa3);
DEFINE_GUID(InputReachedOtherElement_Event_GUID,                       0xed201d8a,0x4e6c,0x415e,0xa8,0x74,0x24,0x60,0xc9,0xb6,0x6b,0xa8);
DEFINE_GUID(InputDiscarded_Event_GUID,                                 0x7f36c367,0x7b18,0x417c,0x97,0xe3,0x9d,0x58,0xdd,0xc9,0x44,0xab);
DEFINE_GUID(SystemAlert_Event_GUID,                                    0xd271545d,0x7a3a,0x47a7,0x84,0x74,0x81,0xd2,0x9a,0x24,0x51,0xc9);
DEFINE_GUID(LiveRegionChanged_Event_GUID,                              0x102d5e90,0xe6a9,0x41b6,0xb1,0xc5,0xa9,0xb1,0x92,0x9d,0x95,0x10);
DEFINE_GUID(HostedFragmentRootsInvalidated_Event_GUID,                 0xe6bdb03e,0x0921,0x4ec5,0x8d,0xcf,0xea,0xe8,0x77,0xb0,0x42,0x6b);
DEFINE_GUID(Drag_DragStart_Event_GUID,                                 0x883a480b,0x3aa9,0x429d,0x95,0xe4,0xd9,0xc8,0xd0,0x11,0xf0,0xdd);
DEFINE_GUID(Drag_DragCancel_Event_GUID,                                0xc3ede6fa,0x3451,0x4e0f,0x9e,0x71,0xdf,0x9c,0x28,0x0a,0x46,0x57);
DEFINE_GUID(Drag_DragComplete_Event_GUID,                              0x38e96188,0xef1f,0x463e,0x91,0xca,0x3a,0x77,0x92,0xc2,0x9c,0xaf);
DEFINE_GUID(DropTarget_DragEnter_Event_GUID,                           0xaad9319b,0x032c,0x4a88,0x96,0x1d,0x1c,0xf5,0x79,0x58,0x1e,0x34);
DEFINE_GUID(DropTarget_DragLeave_Event_GUID,                           0x0f82eb15,0x24a2,0x4988,0x92,0x17,0xde,0x16,0x2a,0xee,0x27,0x2b);
DEFINE_GUID(DropTarget_Dropped_Event_GUID,                             0x622cead8,0x1edb,0x4a3d,0xab,0xbc,0xbe,0x22,0x11,0xff,0x68,0xb5);
DEFINE_GUID(TextEdit_TextChanged_Event_GUID,                           0x120b0308,0xec22,0x4eb8,0x9c,0x98,0x98,0x67,0xcd,0xa1,0xb1,0x65);
DEFINE_GUID(TextEdit_ConversionTargetChanged_Event_GUID,               0x3388c183,0xed4f,0x4c8b,0x9b,0xaa,0x36,0x4d,0x51,0xd8,0x84,0x7f);
DEFINE_GUID(Changes_Event_GUID,                                        0x7df26714,0x614f,0x4e05,0x94,0x88,0x71,0x6c,0x5b,0xa1,0x94,0x36);
DEFINE_GUID(Notification_Event_GUID,                                   0x72c5a2f7,0x9788,0x480f,0xb8,0xeb,0x4d,0xee,0x00,0xf6,0x18,0x6f);

enum AutomationIdentifierType
{
    AutomationIdentifierType_Property,
    AutomationIdentifierType_Pattern,
    AutomationIdentifierType_Event,
    AutomationIdentifierType_ControlType,
    AutomationIdentifierType_TextAttribute,
    AutomationIdentifierType_LandmarkType,
    AutomationIdentifierType_Annotation,
    AutomationIdentifierType_Changes,
    AutomationIdentifierType_Style
};

enum ProviderType
{
    ProviderType_BaseHwnd,
    ProviderType_Proxy,
    ProviderType_NonClientArea,
};

#ifndef __uiautomationclient_h__
enum TreeScope {
    TreeScope_Element     = 0x01,
    TreeScope_Children    = 0x02,
    TreeScope_Descendants = 0x04,
    TreeScope_Parent      = 0x08,
    TreeScope_Ancestors   = 0x10,
    TreeScope_SubTree     = TreeScope_Element | TreeScope_Children | TreeScope_Descendants,
};

enum PropertyConditionFlags {
    PropertyConditionFlags_None           = 0x00,
    PropertyConditionFlags_IgnoreCase     = 0x01,
    PropertyConditionFlags_MatchSubstring = 0x02,
};

enum AutomationElementMode {
    AutomationElementMode_None = 0x00,
    AutomationElementMode_Full = 0x01,
};
#endif

enum ConditionType {
    ConditionType_True     = 0x00,
    ConditionType_False    = 0x01,
    ConditionType_Property = 0x02,
    ConditionType_And      = 0x03,
    ConditionType_Or       = 0x04,
    ConditionType_Not      = 0x05,
};

struct UiaCondition {
    enum ConditionType ConditionType;
};

struct UiaPropertyCondition {
    enum ConditionType ConditionType;
    PROPERTYID PropertyId;
    VARIANT Value;
    enum PropertyConditionFlags Flags;
};

struct UiaAndOrCondition {
    enum ConditionType ConditionType;
    struct UiaCondition **ppConditions;
    int cConditions;
};

struct UiaNotCondition {
    enum ConditionType ConditionType;
    struct UiaCondition *pConditions;
};

struct UiaCacheRequest {
    struct UiaCondition *pViewCondition;
    enum TreeScope Scope;

    PROPERTYID *pProperties;
    int cProperties;
    PATTERNID *pPatterns;
    int cPatterns;

    enum AutomationElementMode automationElementMode;
};

enum EventArgsType {
    EventArgsType_Simple              = 0x00,
    EventArgsType_PropertyChanged     = 0x01,
    EventArgsType_StructureChanged    = 0x02,
    EventArgsType_AsyncContentLoaded  = 0x03,
    EventArgsType_WindowClosed        = 0x04,
    EventArgsType_TextEditTextChanged = 0x05,
    EventArgsType_Changes             = 0x06,
    EventArgsType_Notification        = 0x07,
};

enum AsyncContentLoadedState {
    AsyncContentLoadedState_Beginning = 0x00,
    AsyncContentLoadedState_Progress  = 0x01,
    AsyncContentLoadedState_Completed = 0x02,
};

enum NormalizeState {
    NormalizeState_None   = 0x00,
    NormalizeState_View   = 0x01,
    NormalizeState_Custom = 0x02,
};

struct UiaEventArgs {
    enum EventArgsType Type;
    int EventId;
};

struct UiaPropertyChangedEventArgs {
    enum EventArgsType Type;
    int EventId;

    PROPERTYID PropertyId;
    VARIANT OldValue;
    VARIANT NewValue;
};

struct UiaStructureChangedEventArgs {
    enum EventArgsType Type;
    int EventId;

    enum StructureChangeType StructureChangeType;
    int *pRuntimeId;
    int cRuntimeIdLen;
};

struct UiaTextEditTextChangedEventArgs {
    enum EventArgsType Type;
    int EventId;

    enum TextEditChangeType TextEditChangeType;
    SAFEARRAY *pTextChange;
};

struct UiaChangesEventArgs {
    enum EventArgsType Type;
    int EventId;

    int EventIdCount;
    struct UiaChangeInfo *pUiaChanges;
};

struct UiaAsyncContentLoadedEventArgs {
    enum EventArgsType Type;
    int EventId;

    enum AsyncContentLoadedState AsyncContentLoadedState;
    double percentComplete;
};

struct UiaWindowClosedEventArgs {
    enum EventArgsType Type;
    int EventId;

    int *pRuntimeId;
    int cRuntimeIdLen;
};

struct UiaFindParams {
    int MaxDepth;
    BOOL FindFirst;
    BOOL ExcludeRoot;
    struct UiaCondition *pFindCondition;
};

typedef SAFEARRAY * WINAPI UiaProviderCallback(HWND hwnd,enum ProviderType providerType);
typedef void WINAPI UiaEventCallback(struct UiaEventArgs *pArgs,SAFEARRAY *pRequestedData,BSTR pTreeStructure);

HRESULT WINAPI UiaGetReservedMixedAttributeValue(IUnknown **value);
HRESULT WINAPI UiaGetReservedNotSupportedValue(IUnknown **value);
int WINAPI UiaLookupId(enum AutomationIdentifierType type, const GUID *guid);
BOOL WINAPI UiaPatternRelease(HUIAPATTERNOBJECT hobj);
HRESULT WINAPI UiaRaiseAutomationEvent(IRawElementProviderSimple *provider, EVENTID id);
HRESULT WINAPI UiaRaiseAutomationPropertyChangedEvent(IRawElementProviderSimple *, PROPERTYID, VARIANT, VARIANT);
HRESULT WINAPI UiaRaiseStructureChangedEvent(IRawElementProviderSimple *provider, enum StructureChangeType struct_change_type,
                                             int *runtime_id, int runtime_id_len);
void WINAPI UiaRegisterProviderCallback(UiaProviderCallback *pCallback);
LRESULT WINAPI UiaReturnRawElementProvider(HWND hwnd, WPARAM wParam, LPARAM lParam, IRawElementProviderSimple *elprov);
BOOL WINAPI UiaTextRangeRelease(HUIATEXTRANGE hobj);
HRESULT WINAPI UiaHostProviderFromHwnd(HWND hwnd, IRawElementProviderSimple **elprov);
HRESULT WINAPI UiaProviderFromIAccessible(IAccessible *acc, long child_id, DWORD flags, IRawElementProviderSimple **elprov);
HRESULT WINAPI UiaGetPropertyValue(HUIANODE huianode, PROPERTYID prop_id, VARIANT *out_val);
HRESULT WINAPI UiaNodeFromProvider(IRawElementProviderSimple *elprov, HUIANODE *huianode);
BOOL WINAPI UiaNodeRelease(HUIANODE huianode);
HRESULT WINAPI UiaGetRuntimeId(HUIANODE huianode, SAFEARRAY **runtime_id);
HRESULT WINAPI UiaHUiaNodeFromVariant(VARIANT *in_val, HUIANODE *huianode);
HRESULT WINAPI UiaNodeFromHandle(HWND hwnd, HUIANODE *huianode);
HRESULT WINAPI UiaDisconnectProvider(IRawElementProviderSimple *elprov);
HRESULT WINAPI UiaGetUpdatedCache(HUIANODE huianode, struct UiaCacheRequest *cache_req, enum NormalizeState normalize_state,
        struct UiaCondition *normalize_cond, SAFEARRAY **out_req, BSTR *tree_struct);
HRESULT WINAPI UiaNavigate(HUIANODE huianode, enum NavigateDirection dir, struct UiaCondition *nav_condition,
        struct UiaCacheRequest *cache_req, SAFEARRAY **out_req, BSTR *tree_struct);
HRESULT WINAPI UiaFind(HUIANODE huianode, struct UiaFindParams *find_params, struct UiaCacheRequest *cache_req, SAFEARRAY **out_req,
        SAFEARRAY **out_offsets, SAFEARRAY **out_tree_structs);
HRESULT WINAPI UiaAddEvent(HUIANODE huianode, EVENTID event_id, UiaEventCallback *callback, enum TreeScope scope,
        PROPERTYID *prop_ids, int prop_ids_count, struct UiaCacheRequest *cache_req, HUIAEVENT *huiaevent);
HRESULT WINAPI UiaRemoveEvent(HUIAEVENT huiaevent);
HRESULT WINAPI UiaEventAddWindow(HUIAEVENT huiaevent, HWND hwnd);
HRESULT WINAPI UiaEventRemoveWindow(HUIAEVENT huiaevent, HWND hwnd);
HRESULT WINAPI UiaProviderForNonClient(HWND hwnd, long objid, long child_id, IRawElementProviderSimple **elprov);

#ifdef __cplusplus
}
#endif

#endif /* _INC_UIAUTOMATIONCOREAPI */
