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
    /// �������չ
    /// </summary>
    public class GridColumnEx : FCGridColumn
    {
        public GridColumnEx()
        {
            setBorderColor(FCColor.rgba(0, 0, 0, 100));
        }

        /// <summary>
        /// �ػ汳������
        /// </summary>
        /// <param name="paint">��ͼ����</param>
        /// <param name="clipRect">�ü�����</param>
        public override void onPaintBackground(FCPaint paint, FCRect clipRect)
        {
            int width = getWidth(), height = getHeight();
            FCRect drawRect = new FCRect(0, 0, width, height);
            paint.fillGradientRect(FCColor.rgb(255, 255, 255), FCColor.rgb(240, 240, 240), drawRect, 0, 90);
        }
    }
}
