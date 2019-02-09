using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Game_Editor
{
    public partial class Form1 : Form
    {
        [DllImport(@"C:\DMAP\game\GameEngine-master\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetWndHnd@@YGXPAUHWND__@@@Z")]
        public extern static unsafe void SetWndHandler(IntPtr wndHandler);

        [DllImport(@"C:\DMAP\game\GameEngine-master\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetWndParams@@YGXHH@Z")]
        public extern static unsafe void SetWndParameters(int width, int height);



        public Form1()
        {
            InitializeComponent();
        }

        private unsafe void Form1_Load(object sender, EventArgs e)
        {

        }

        private unsafe void button1_Click(object sender, EventArgs e)
        {
            IntPtr panelHandler = (IntPtr)panel1.Handle.ToPointer();

            int screenWidth = panel1.Width;
            int screenHeight = panel1.Height;

            SetWndParameters(screenWidth, screenHeight);

            SetWndHandler(panelHandler);
        }
    }
}
