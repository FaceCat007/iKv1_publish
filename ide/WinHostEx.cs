/*����FaceCat(����è)��� v1.0 https://github.com/FaceCat007/facecat.git*/

using System;
using System.Collections.Generic;
using System.Text;
using FaceCat;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

namespace FaceCat {
    /// <summary>
    /// ���������
    /// </summary>
    public class WinHostEx : WinHost {
        [Flags]
        enum MouseEventFlag : uint {
            Move = 0x0001,
            LeftDown = 0x0002,
            LeftUp = 0x0004,
            RightDown = 0x0008,
            RightUp = 0x0010,
            MiddleDown = 0x0020,
            MiddleUp = 0x0040,
            XDown = 0x0080,
            XUp = 0x0100,
            Wheel = 0x0800,
            VirtualDesk = 0x4000,
            Absolute = 0x8000
        }

        public const int WM_KEYDOWN = 0x0100;
        public const int WM_KEYUP = 0x0101;
        public const int WM_CHAR = 0x0102;
        public const int WM_SYSKEYDOWN = 0x0104;
        public const int WM_SYSKEYUP = 0x0105;
        public const int WM_SYSCHAR = 0x0106;

        [DllImport("user32.dll")]
        private static extern bool SetCursorPos(int X, int Y);

        [DllImport("user32.dll")]
        private static extern void mouse_event(MouseEventFlag flags, int dx, int dy, int data, int extraInfo);

        [DllImport("user32.dll", EntryPoint = "SendMessageA")]
        private static extern int SendMessage(int hwnd, int wMsg, int wParam, int lParam);

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        private static extern int SendMessage(int hWnd, int Msg, int wParam, String lParam);

        [DllImport("User32.dll", EntryPoint = "SendMessage")]
        private static extern int SendMessage(int hWnd, int Msg, int wParam, StringBuilder lParam);

        [DllImport("user32.dll", EntryPoint = "WindowFromPoint")]
        private static extern IntPtr WindowFromPoint(int xPoint, int yPoint);

        [DllImport("user32.dll", EntryPoint = "GetCursorPos")]
        public static extern bool GetCursorPos(ref FCPoint lpPoint);

        [DllImport("User32.dll", EntryPoint = "FindWindow")]
        private static extern int FindWindow(String lpClassName, String lpWindowName);

        /// <summary>
        /// �����ڲ��ؼ�
        /// </summary>
        /// <param name="parent">���ؼ�</param>
        /// <param name="clsid">�ؼ���ʶ</param>
        /// <returns>�ڲ��ؼ�</returns>
        public override FCView createInternalView(FCView parent, String clsid) {
            //�����ؼ�
            FCCalendar calendar = parent as FCCalendar;
            if (calendar != null) {
                if (clsid == "datetitle") {
                    return new DateTitle(calendar);
                }
                else if (clsid == "headdiv") {
                    HeadDiv headDiv = new HeadDiv(calendar);
                    headDiv.setWidth(parent.getWidth());
                    headDiv.setDock(FCDockStyle.Top);
                    return headDiv;
                }
                else if (clsid == "lastbutton") {
                    return new ArrowButton(calendar);
                }
                else if (clsid == "nextbutton") {
                    ArrowButton nextBtn = new ArrowButton(calendar);
                    nextBtn.setToLast(false);
                    return nextBtn;
                }
            }
            //�ָ��
            FCSplitLayoutDiv splitLayoutDiv = parent as FCSplitLayoutDiv;
            if (splitLayoutDiv != null) {
                if (clsid == "splitter") {
                    FCButton splitter = new FCButton();
                    splitter.setBackColor(FCColor.rgba(50, 50, 50, 100));
                    splitter.setBorderColor(FCColor.rgba(50, 50, 50, 100));
                    splitter.setSize(new FCSize(5, 5));
                    return splitter;
                }
            }
            //������
            FCScrollBar scrollBar = parent as FCScrollBar;
            if (scrollBar != null) {
                scrollBar.setBorderColor(FCColor.None);
                scrollBar.setBackColor(FCColor.None);
                if (clsid == "addbutton") {
                    FCButton addButton = new FCButton();
                    addButton.setSize(new FCSize(0, 0));
                    return addButton;
                }
                else if (clsid == "backbutton") {
                    FCButton backButton = new FCButton();
                    backButton.setBorderColor(FCColor.None);
                    backButton.setBackColor(FCColor.None);
                    return backButton;
                }
                else if (clsid == "scrollbutton") {
                    FCButton scrollButton = new FCButton();
                    scrollButton.setAllowDrag(true);
                    scrollButton.setBackColor(FCColor.rgba(100, 100, 100, 100));
                    scrollButton.setBorderColor(FCColor.rgba(50, 50, 50, 100));
                    return scrollButton;
                }
                else if (clsid == "reducebutton") {
                    FCButton reduceButton = new FCButton();
                    reduceButton.setSize(new FCSize(0, 0));
                    return reduceButton;
                }
            }
            //ҳ��
            FCTabPage tabPage = parent as FCTabPage;
            if (tabPage != null) {
                if (clsid == "headerbutton") {
                    FCButton button = new FCButton();
                    button.setAllowDrag(true);
                    FCSize size = new FCSize(100, 30);
                    button.setSize(size);
                    return button;
                }
            }
            //�����б�
            FCComboBox comboBox = parent as FCComboBox;
            if (comboBox != null) {
                if (clsid == "dropdownbutton") {
                    FCButton dropDownButton = new FCButton();
                    dropDownButton.setDisplayOffset(false);
                    int width = comboBox.getWidth();
                    int height = comboBox.getHeight();
                    FCPoint location = new FCPoint(width - 20, 0);
                    dropDownButton.setLocation(location);
                    FCSize size = new FCSize(20, height);
                    dropDownButton.setSize(size);
                    return dropDownButton;
                }
                else if (clsid == "dropdownmenu") {
                    FCComboBoxMenu comboBoxMenu = new FCComboBoxMenu();
                    comboBoxMenu.setComboBox(comboBox);
                    comboBoxMenu.setPopup(true);
                    FCSize size = new FCSize(100, 200);
                    comboBoxMenu.setSize(size);
                    return comboBoxMenu;
                }
            }
            //����ѡ��
            FCDateTimePicker datePicker = parent as FCDateTimePicker;
            if (datePicker != null) {
                if (clsid == "dropdownbutton") {
                    FCButton dropDownButton = new FCButton();
                    dropDownButton.setDisplayOffset(false);
                    int width = datePicker.getWidth();
                    int height = datePicker.getHeight();
                    FCPoint location = new FCPoint(width - 16, 0);
                    dropDownButton.setLocation(location);
                    FCSize size = new FCSize(16, height);
                    dropDownButton.setSize(size);
                    return dropDownButton;
                }
                else if (clsid == "dropdownmenu") {
                    FCMenu dropDownMenu = new FCMenu();
                    dropDownMenu.setPadding(new FCPadding(1));
                    dropDownMenu.setPopup(true);
                    FCSize size = new FCSize(200, 200);
                    dropDownMenu.setSize(size);
                    return dropDownMenu;
                }
            }
            //����ѡ��
            FCSpin spin = parent as FCSpin;
            if (spin != null) {
                if (clsid == "downbutton") {
                    FCButton downButton = new FCButton();
                    downButton.setDisplayOffset(false);
                    FCSize size = new FCSize(16, 16);
                    downButton.setSize(size);
                    return downButton;
                }
                else if (clsid == "upbutton") {
                    FCButton upButton = new FCButton();
                    upButton.setDisplayOffset(false);
                    FCSize size = new FCSize(16, 16);
                    upButton.setSize(size);
                    return upButton;
                }
            }
            //������
            FCDiv div = parent as FCDiv;
            if (div != null) {
                if (clsid == "hscrollbar") {
                    FCHScrollBar hScrollBar = new FCHScrollBar();
                    hScrollBar.setVisible(false);
                    hScrollBar.setSize(new FCSize(10, 10));
                    return hScrollBar;
                }
                else if (clsid == "vscrollbar") {
                    FCVScrollBar vScrollBar = new FCVScrollBar();
                    vScrollBar.setVisible(false);
                    vScrollBar.setSize(new FCSize(10, 10));
                    return vScrollBar;
                }
            }
            //���
            FCGrid grid = parent as FCGrid;
            if (grid != null) {
                if (clsid == "edittextbox") {
                    FCTextBox textBox = new FCTextBox();
                    textBox.setFont(new FCFont("����", 18, false, false, false));
                    textBox.setPadding(new FCPadding(20, 0, 0, 0));
                    return textBox;
                }
            }
            return base.createInternalView(parent, clsid);
        }

        /// <summary>
        /// ִ�г���
        /// </summary>
        /// <param name="cmd">����</param>
        public static void execute(String cmd) {
            try {
                Process.Start(cmd);
            }
            catch { }
        }

        /// <summary>
        /// ��ȡ�ı�
        /// </summary>
        /// <returns>�ı�</returns>
        public static String getText() {
            FCPoint mp = new FCPoint();
            GetCursorPos(ref mp);
            IntPtr handle = WindowFromPoint(mp.x, mp.y);
            if (handle != IntPtr.Zero) {
                StringBuilder sb = new StringBuilder(10240);
                SendMessage((int)handle, 0xD, 10240, sb);
                return sb.ToString();
            }
            return "";
        }

        /// <summary>
        /// ��������¼�
        /// </summary>
        /// <param name="eventName">�¼�ID</param>
        /// <param name="dx">������</param>
        /// <param name="dy">������</param>
        /// <param name="data">����ֵ</param>
        public static void mouseEvent(String eventName, int dx, int dy, int data) {
            MouseEventFlag flag = MouseEventFlag.Move;
            if (eventName == "SETCURSOR") {
                SetCursorPos(dx, dy);
                return;
            }
            else if (eventName == "MOVE") {
                flag = MouseEventFlag.Move;
            }
            else if (eventName == "LEFTDOWN") {
                flag = MouseEventFlag.LeftDown;
            }
            else if (eventName == "LEFTUP") {
                flag = MouseEventFlag.LeftUp;
            }
            else if (eventName == "RIGHTDOWN") {
                flag = MouseEventFlag.RightDown;
            }
            else if (eventName == "RIGHTUP") {
                flag = MouseEventFlag.RightUp;
            }
            else if (eventName == "MIDDLEDOWN") {
                flag = MouseEventFlag.MiddleDown;
            }
            else if (eventName == "MIDDLEUP") {
                flag = MouseEventFlag.MiddleUp;
            }
            else if (eventName == "XDOWN") {
                flag = MouseEventFlag.XDown;
            }
            else if (eventName == "XUP") {
                flag = MouseEventFlag.XUp;
            }
            else if (eventName == "WHEEL") {
                flag = MouseEventFlag.Wheel;
            }
            else if (eventName == "VIRTUALDESK") {
                flag = MouseEventFlag.VirtualDesk;
            }
            else if (eventName == "ABSOLUTE") {
                flag = MouseEventFlag.Absolute;
            }
            mouse_event(flag, dx, dy, data, 0);
        }

        /// <summary>
        /// ���������¼�
        /// </summary>
        /// <param name="key">����</param>
        public static void sendKey(String key) {
            SendKeys.Send(key);
        }

        /// <summary>
        /// ��������
        /// </summary>
        /// <param name="text">����</param>
        public static void setText(String text) {
            if (text != null && text.Length > 0) {
                Clipboard.Clear();
                Clipboard.SetText(text);
                SendKeys.SendWait("^v");
            }
        }

        /// <summary>
        /// ��ʾToolTip
        /// </summary>
        /// <param name="text">����</param>
        /// <param name="mp">λ��</param>
        public override void showToolTip(String text, FCPoint mp) {
            //toolTip.Show(text, Control.FromHandle(HWnd), new Point(mp.x, mp.y));
            base.showToolTip(text, mp);
        }
    }
}
