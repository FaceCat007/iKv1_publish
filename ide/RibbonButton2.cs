/*基于捂脸猫FaceCat框架 v1.0
上海卷卷猫信息技术有限公司
 */

using System;
using System.Collections.Generic;
using System.Text;

namespace FaceCat {
    /// <summary>
    /// 透明按钮
    /// </summary>
    public class RibbonButton2 : FCButton {
        /// <summary>
        /// 创建按钮
        /// </summary>
        public RibbonButton2() {
            setBorderColor(FCColor.None);
            setTextColor (FCColor.rgba(255, 255, 255, 200));
            setFont(new FCFont("黑体", 14, false, false, false));
        }

        private int m_timerID = FCView.getNewTimerID();

        private int m_tick = 0;

        public override void onTouchEnter(FCTouchInfo touchInfo)
        {
            base.onTouchEnter(touchInfo);
            m_tick = 60;
            startTimer(m_timerID, 10);
        }

        public override void onTimer(int timerID)
        {
            base.onTimer(timerID);
            if (m_tick > 0)
            {
                m_tick--;
                if (m_tick == 0)
                {
                    stopTimer(m_timerID);
                }
                invalidate();
            }
        }

        /// <summary>
        /// 重绘背景方法
        /// </summary>
        /// <param name="paint">绘图对象</param>
        /// <param name="clipRect">裁剪区域</param>
        public override void onPaint(FCPaint paint, FCRect clipRect) {
            GdiPlusPaintEx gdiPlusPaintEx = paint as GdiPlusPaintEx;
            int width = getWidth(), height = getHeight();
            FCRect drawRect = new FCRect(1, 1, width - 1, height - 1);
            FCFont tFont = getFont();
            FCSize tSize = paint.textSize(getText(), tFont);
            FCRect tRect = new FCRect((width - tSize.cx) / 2, (height - tSize.cy) / 2 + 1, (width + tSize.cx) / 2, (height + tSize.cy) / 2 + 1);
            FCPoint[] points = new FCPoint[5];
            points[0] = new FCPoint(drawRect.left, drawRect.top);
            points[1] = new FCPoint(drawRect.right - 10, drawRect.top);
            points[2] = new FCPoint(drawRect.right, drawRect.top + 10);
            points[3] = new FCPoint(drawRect.right, drawRect.bottom);
            points[4] = new FCPoint(drawRect.left, drawRect.bottom);
            paint.drawPolygon(FCColor.rgba(0, 0, 0, 150), 1, 0, points);
            String text = getText();
            double scaleFactorX = 0, scaleFactorY = 0;
            paint.getScaleFactor(ref scaleFactorX, ref scaleFactorY);
            if (scaleFactorX != 1.0 || scaleFactorY != 1.0)
            {
                if (m_tick > 0)
                {
                    stopTimer(m_timerID);
                    m_tick = 0;
                }
            }
            if (text == "新增行")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(0, 0, 0, 200), tFont, tRect);
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                paint.drawLine(FCColor.rgb(219, 68, 83), 3, 0, drawRect.right - 15 + offsetX, drawRect.top + 10 + offsetY, drawRect.right - 15 + offsetX, drawRect.bottom - 10 + offsetY);
                paint.drawLine(FCColor.rgb(219, 68, 83), 3, 0, drawRect.right - 24 + offsetX, drawRect.top + 19 + offsetY, drawRect.right - 6 + offsetX, drawRect.top + 19 + offsetY);
                if(m_tick > 0){
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else if (text == "删除行")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(0, 0, 0, 200), tFont, tRect);
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                paint.drawLine(FCColor.rgb(140, 192, 81), 3, 0, drawRect.right - 24 + offsetX, drawRect.top + 19 + offsetY, drawRect.right - 6 + offsetX, drawRect.top + 19 + offsetY);
                if (m_tick > 0)
                {
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else if (text == "加载或创建库")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(0, 0, 0, 200), tFont, tRect);
                FCRect blockRect = new FCRect(drawRect.right - 24, drawRect.top + 10, drawRect.right - 6, drawRect.bottom - 10);         
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                FCPoint[] pp = new FCPoint[3];
                pp[0] = new FCPoint(blockRect.left + offsetX, blockRect.top + 8 + offsetY);
                pp[1] = new FCPoint(blockRect.left + 6 + offsetX, blockRect.bottom + offsetY);
                pp[2] = new FCPoint(blockRect.right + offsetX - 1, blockRect.top + offsetY);
                paint.drawPolyline(FCColor.rgb(59, 174, 218), 2, 0, pp);
                if (m_tick > 0)
                {
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else if (text == "打开")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(0, 0, 0, 200), tFont, tRect);
                FCRect blockRect = new FCRect(drawRect.right - 24, drawRect.top + 10, drawRect.right - 6, drawRect.bottom - 10);
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                FCPoint[] pp = new FCPoint[4];
                pp[0] = new FCPoint(blockRect.left + offsetX, blockRect.bottom + offsetY);
                pp[1] = new FCPoint(blockRect.left + 6 + offsetX, blockRect.top + offsetY);
                pp[2] = new FCPoint(blockRect.right + offsetX - 1, blockRect.bottom + offsetY);
                pp[3] = new FCPoint(blockRect.right - 10 + offsetX - 1, blockRect.bottom - 6 + offsetY);
                paint.drawPolyline(FCColor.rgb(216, 112, 173), 2, 0, pp);
                if (m_tick > 0)
                {
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else if (text == "查看教程")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(255, 255, 255, 200), tFont, tRect);
                
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                FCRect blockRect = new FCRect(drawRect.right - 24 + offsetX, drawRect.top + 10 + offsetY, drawRect.right - 6 + offsetX, drawRect.bottom - 10 + offsetY);
                paint.drawArc(FCColor.rgb(246, 187, 67), 2, 0, blockRect, 20, 270);
                FCRect innerRect = new FCRect(blockRect.left + 5, blockRect.top + 5, blockRect.left + 10, blockRect.top + 10);
                paint.fillEllipse(FCColor.rgb(246, 187, 67), innerRect);
                if (m_tick > 0)
                {
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else if (text == "关于我们")
            {
                tRect.left -= 10;
                tRect.right -= 10;
                paint.drawText(text, FCColor.rgba(255, 255, 255, 200), tFont, tRect);
                FCRect blockRect = new FCRect(drawRect.right - 24, drawRect.top + 10, drawRect.right - 6, drawRect.bottom - 10);        
                int offsetX = 0, offsetY = 0;
                FCPoint offset = paint.getOffset();
                if (m_tick > 0)
                {
                    offsetX = -(drawRect.right - 15);
                    offsetY = -(drawRect.top + 19);
                    int clientX = m_native.clientX(this);
                    int clientY = m_native.clientY(this);
                    paint.setOffset(new FCPoint(0, 0));
                    gdiPlusPaintEx.m_g.TranslateTransform(clientX - offsetX, clientY - offsetY);
                    gdiPlusPaintEx.m_g.RotateTransform(m_tick * 10);
                }
                FCPoint[] pp = new FCPoint[6];
                pp[0] = new FCPoint(blockRect.right + offsetX - 1, blockRect.top + offsetY);
                pp[1] = new FCPoint(blockRect.left + offsetX, blockRect.bottom + offsetY);
                pp[2] = new FCPoint(blockRect.left + offsetX, blockRect.top + offsetY);
                pp[3] = new FCPoint(blockRect.right + offsetX - 1, blockRect.top + offsetY);
                pp[4] = new FCPoint(blockRect.right + offsetX - 1, blockRect.bottom + offsetY);
                pp[5] = new FCPoint(blockRect.left + offsetX, blockRect.top + offsetY);
                paint.drawPolyline(FCColor.rgb(170, 178, 189), 2, 0, pp);
                if (m_tick > 0)
                {
                    gdiPlusPaintEx.m_g.ResetTransform();
                    paint.setOffset(offset);
                }
            }
            else
            {
                paint.drawText(text, FCColor.rgba(255, 255, 255, 200), tFont, tRect);
            }
            if (this == getNative().getPushedView())
            {
                paint.fillPolygon(FCColor.rgba(255, 255, 255, 50), points);
            }
        }
    }
}
