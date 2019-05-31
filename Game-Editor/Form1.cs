using System;
using System.Windows.Forms;

namespace Game_Editor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            Application.AddMessageFilter(new KeyboardMessageFilter());
        }

        private unsafe void Form1_Load(object sender, EventArgs e)
        {
            this.Focus();
        }

        private unsafe void button1_Click(object sender, EventArgs e)
        {
            IntPtr panelHandler = (IntPtr)panel1.Handle.ToPointer();

            int screenWidth = panel1.Width;
            int screenHeight = panel1.Height;

            RenderEngineAPI.SetWndParameters(screenWidth, screenHeight);

            RenderEngineAPI.SetWndHandler(panelHandler);
        }

        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {               
            //System.Console.WriteLine("{0} {1}", e.Location.X, e.Location.Y);
            RenderEngineAPI.SetMousePosition(e.Location.X, e.Location.Y);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            RenderEngineAPI.SetKeyPressed((char)'w');
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            RenderEngineAPI.SetKeyPressed(e.KeyValue);
        }

        private void textBox1_KeyUp(object sender, KeyEventArgs e)
        {
            RenderEngineAPI.SetKeyReleased(e.KeyValue);
        }

        private int findSelectedButton(MouseButtons buttonClick)
        {
            int[] button = { (int)MouseButtons.Left, (int)MouseButtons.Right, (int)MouseButtons.Middle };
            int retVal = 0;
            for (int i = 0; i < button.Length; ++i)
            {
                if ((int)buttonClick == button[i])
                {
                    retVal = i;
                    break;
                }
            }

            return retVal;
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            int selectedButton = findSelectedButton(e.Button);

            RenderEngineAPI.SetMouseAction(e.Location.X, e.Location.Y, selectedButton, 0);
            
        }

        private void panel1_MouseUp(object sender, MouseEventArgs e)
        {
            int selectedButton = findSelectedButton(e.Button);

            RenderEngineAPI.SetMouseAction(e.Location.X, e.Location.Y, selectedButton, 1);
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            MessageBox.Show("test");
        }
    }

    class KeyboardMessageFilter : IMessageFilter
    {
        public bool PreFilterMessage(ref Message m)
        {
            switch (m.Msg)
            {
                case (0x100): // KEY DOWN
                    //RenderEngineAPI.SetKeyPressed((int)m.WParam);
                    return true;
                case (0x101): // KEY UP
                    //RenderEngineAPI.SetKeyReleased((int)m.WParam);
                    return true;
                default:
                    break;
            }

            return false;
        }
    }


}
