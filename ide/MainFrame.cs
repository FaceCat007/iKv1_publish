using System;
using System.Collections.Generic;
using System.Text;
using FaceCat;

namespace FaceCat
{
    public class MainFrame : FCUIXml
    {
        public override FCView createView(System.Xml.XmlNode node, string type)
        {
            if (type == "column" || type == "th")
            {
                return new GridColumnEx();
            }
            else if (type == "ribbonbutton2")
            {
                return new RibbonButton2();
            } 
            else if (type == "myspin")
            {
                return new MySpin();
            }
            return base.createView(node, type);
        }

        public override void loadFile(string fileName, FCView view)
        {
            base.loadFile(fileName, view);
        }
    }

    /// <summary>
    /// 表格列扩展
    /// </summary>
    public class GridColumnEx : FCGridColumn
    {
        public GridColumnEx()
        {
            setBorderColor(FCColor.rgba(0, 0, 0, 100));
        }

        /// <summary>
        /// 重绘背景方法
        /// </summary>
        /// <param name="paint">绘图区域</param>
        /// <param name="clipRect">裁剪对象</param>
        public override void onPaintBackground(FCPaint paint, FCRect clipRect)
        {
            int width = getWidth(), height = getHeight();
            FCRect drawRect = new FCRect(0, 0, width, height);
            paint.fillGradientRect(FCColor.rgb(255, 255, 255), FCColor.rgb(240, 240, 240), drawRect, 0, 90);
        }
    }
}
