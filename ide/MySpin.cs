using System;
using System.Collections.Generic;
using System.Text;

namespace FaceCat
{
    public class MySpin : FCSpin
    {
        /// <summary>
        /// 创建Spin
        /// </summary>
        public MySpin()
        {
            setTextAlign(FCHorizontalAlign.Center);
            setBackColor(FCColor.rgba(44, 59, 66, 200));
            setTextColor(FCColor.rgba(255, 255, 255, 255));
            setMaximum(1000000);
            setCornerRadius(8);
        }

        public String m_unit = "";

        /// <summary>
        /// 重绘方法
        /// </summary>
        /// <param name="paint"></param>
        /// <param name="clipRect"></param>
        public override void onPaint(FCPaint paint, FCRect clipRect)
        {
            int width = getWidth(), height = getHeight();
            //base.onPaint(paint, clipRect);
            FCRect drawRect = new FCRect(0, 0, width, height);
            paint.fillRoundRect(getPaintingBackColor(), drawRect, getCornerRadius());
            FCFont tFont = getFont();
            String str = ((int)getValue()).ToString() + m_unit;
            if (m_unit == "X")
            {
                str = ((int)getValue()).ToString() + " X " + ((int)getValue()).ToString();
            }
            FCSize tSize = paint.textSize(str, tFont);
            long textColor = getTextColor();
            FCRect tRect = new FCRect((width - tSize.cx) / 2, (height - tSize.cy) / 2, (width + tSize.cx) / 2, (height + tSize.cy) / 2);
            paint.drawText(str, textColor, tFont, tRect);

        }

        public override void onPaintBorder(FCPaint paint, FCRect clipRect)
        {
        }

        /// <summary>
        /// 添加视图方法
        /// </summary>
        public override void onLoad()
        {
            FCHost host = getNative().getHost();
            m_unit = "个";
            if (m_downButton == null)
            {
                m_downButton = new FCButton();
                m_downButton.addEvent(this, "ontouchdown", null);
                m_downButton.addEvent(this, "ontouchup", null);
                m_downButton.setBackColor(FCColor.rgba(70, 156, 143, 255));
                m_downButton.setFont(new FCFont("黑体", 18, false, false, false));
                m_downButton.setText("-");
                m_downButton.setCornerRadius(8);
                addView(m_downButton);
            }
            if (m_upButton == null)
            {
                m_upButton = new FCButton();
                m_upButton.addEvent(this, "ontouchdown", null);
                m_upButton.addEvent(this, "ontouchup", null);
                m_upButton.setBackColor(FCColor.rgba(70, 156, 143, 255));
                m_upButton.setFont(new FCFont("黑体", 18, false, false, false));
                m_upButton.setText("+");
                m_upButton.setCornerRadius(8);
                addView(m_upButton);
            }
        }

        /// <summary>
        /// 更新布局方法
        /// </summary>
        public override void update()
        {
            //base.update();
            int width = getWidth(), height = getHeight();
            if (m_upButton != null)
            {
                int uWidth = getHeight();
                FCPoint location = new FCPoint(width - uWidth, 0);
                m_upButton.setLocation(location);
                FCSize size = new FCSize(uWidth, height);
                m_upButton.setSize(size);
            }
            if (m_downButton != null)
            {
                int dWidth = getHeight();
                FCPoint location = new FCPoint(0, 0);
                m_downButton.setLocation(location);
                FCSize size = new FCSize(dWidth, height);
                m_downButton.setSize(size);
            }
        }
    }
}
