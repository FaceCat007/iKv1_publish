using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace FaceCat {
    public partial class Form1 : Form, FCTouchEventCallBack , FCGridCellEventCallBack{
        public Form1() {
            InitializeComponent();
            SetStyle(ControlStyles.ResizeRedraw, true);
        }

        private WinHost m_host;

        private FCNative m_native;

        private MainFrame m_mainFrame;

        private FCGrid m_gridDB;

        protected override void OnLoad(EventArgs e) {
            base.OnLoad(e);
            m_mainFrame = new MainFrame();
            m_mainFrame.createNative();
            m_native = m_mainFrame.getNative();
            m_native.setPaint(new GdiPlusPaintEx());
            m_host = new WinHostEx();
            m_native.setHost(m_host);
            m_host.setNative(m_native);
            m_host.setHWnd(Handle);
            m_native.setSize(new FCSize(ClientSize.Width, ClientSize.Height));
            m_mainFrame.loadFile(Application.StartupPath + "\\MainFrame.html", null);
            m_native.update();
            m_mainFrame.getButton("btnDir").addEvent(this, "onclick", this);
            m_mainFrame.getButton("btnNewDB").addEvent(this, "onclick", this);
            m_mainFrame.getButton("btnNewRow").addEvent(this, "onclick", this);
            m_mainFrame.getButton("btnDeleteRow").addEvent(this, "onclick", this);
            m_txtPath = m_mainFrame.getTextBox("txtPath");
            m_txtDBName = m_mainFrame.getTextBox("txtDBName");
            m_spThread = m_mainFrame.getSpin("spThread");
            m_gridDB = m_mainFrame.getGrid("gridDB");
            m_gridDB.addEvent(this, "ongridcelleditend", this);
            FCGridRowStyle rowStyle = new FCGridRowStyle();
            rowStyle.setFont(new FCFont("ºÚÌå", 14));
            rowStyle.setSelectedBackColor(FCColor.rgba(200, 200, 200, 255));
            rowStyle.setHoveredBackColor(FCColor.None);
            m_gridDB.setRowStyle(rowStyle);

            FCGridRowStyle alternateRowStyle = new FCGridRowStyle();
            alternateRowStyle.setFont(new FCFont("ºÚÌå", 14));
            alternateRowStyle.setBackColor(FCColor.rgba(240, 240, 240, 255));
            alternateRowStyle.setSelectedBackColor(FCColor.rgba(200, 200, 200, 255));
            alternateRowStyle.setHoveredBackColor(FCColor.None);
            m_gridDB.setAlternateRowStyle(alternateRowStyle);
        }

        private FCTextBox m_txtPath;

        private FCTextBox m_txtDBName;

        private FCSpin m_spThread;

        private int m_dbID = -1;

        protected override void WndProc(ref Message m) {
            if (m_host != null && m_host.onMessage(ref m) > 0) {
                return;
            }
            base.WndProc(ref m);
        }

        public void callTouchEvent(string eventName, object sender, FCTouchInfo touchInfo, object invoke)
        {
            if (eventName == "onclick")
            {
                FCView view = sender as FCView;
                String name = view.getName();
                if (name == "btnDir")
                {
                    FolderBrowserDialog fdb = new FolderBrowserDialog();
                    if (DialogResult.OK == fdb.ShowDialog())
                    {
                        m_txtPath.setText(fdb.SelectedPath);
                        m_txtPath.invalidate();
                        String dir = m_txtPath.getText();
                        String dbName = m_txtDBName.getText();
                        int thread = (int)m_spThread.getValue();
                        if (FCFile.isDirectoryExist(dir))
                        {
                            ArrayList<String> files = new ArrayList<string>();
                            FCFile.getFiles(dir, files);
                            for (int i = 0; i < files.size(); i++)
                            {
                                String fName = files.get(i);
                                if (fName.IndexOf("\\info.txt") != -1)
                                {
                                    String content = "";
                                    FCFile.read(fName, ref content);
                                    thread = FCTran.strToInt(content);
                                    m_spThread.setValue(thread);
                                }
                                else if (fName.IndexOf(".db") != -1)
                                {
                                    String dName = fName.Substring(fName.LastIndexOf("\\") + 1);
                                    dbName = dName.Substring(0, dName.IndexOf("_"));
                                    m_txtDBName.setText(dbName);
                                }
                            }
                        }
                        m_native.invalidate();
                    }
                }
                else if (name == "btnNewDB")
                {
                    String dir = m_txtPath.getText();
                    String dbName = m_txtDBName.getText();
                    int thread = (int)m_spThread.getValue();
                    KVDataBase.closeAllKvs();
                    m_dbID = KVDataBase.initKv(dir, dbName, thread);
                    m_gridDB.beginUpdate();
                    for (int i = 0; i < thread; i++)
                    {
                        int kvCount = KVDataBase.getKvCount(m_dbID, i);
                        for (int j = 0; j < kvCount; j++)
                        {
                            String key = "", value = "";
                            KVDataBase.getKv2(m_dbID, j, i, ref key, ref value);
                            FCGridRow row = new FCGridRow();
                            row.setHeight(30);
                            m_gridDB.addRow(row);
                            FCGridStringCell valueCell = new FCGridStringCell(value);
                            valueCell.setAllowEdit(true);
                            row.addCell("colKey", new FCGridStringCell(key));
                            row.addCell("colValue", valueCell);
                        }
                    }
                    m_gridDB.endUpdate();
                    m_native.invalidate();
                }
                else if (name == "btnNewRow")
                {
                    String newKey = System.Guid.NewGuid().ToString();
                    String newValue = "--";
                    KVDataBase.putKv(m_dbID, newKey, newValue);
                    FCGridRow row = new FCGridRow();
                    row.setHeight(30);
                    m_gridDB.insertRow(0, row);
                    FCGridStringCell valueCell = new FCGridStringCell(newValue);
                    valueCell.setAllowEdit(true);
                    row.addCell("colKey", new FCGridStringCell(newKey));
                    row.addCell("colValue", valueCell);
                    m_gridDB.update(); 
                    m_gridDB.invalidate();
                }
                else if (name == "btnDeleteRow")
                {
                    ArrayList<FCGridRow> selectedRows = m_gridDB.getSelectedRows();
                    if (selectedRows.size() > 0)
                    {
                        FCGridRow selectedRow = selectedRows.get(0);
                        String key = selectedRow.getCell("colKey").getString();
                        KVDataBase.removeKv(m_dbID, key);
                        m_gridDB.removeRow(selectedRow);
                        selectedRow.delete();
                        m_gridDB.update(); 
                        m_gridDB.invalidate();
                    }
                }
            }
        }

        public void callGridCellEvent(string eventName, object sender, FCGridCell cell, object invoke)
        {
            if (eventName == "ongridcelleditend")
            {
                FCGridRow row = cell.getRow();
                String key = row.getCell("colKey").getString();
                String newValue = row.getCell("colValue").getString();
                KVDataBase.putKv(m_dbID, key, newValue);
            }
        }
    }
}