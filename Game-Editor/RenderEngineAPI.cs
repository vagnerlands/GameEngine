using System;
using System.Runtime.InteropServices;

namespace Game_Editor
{
    class RenderEngineAPI
    {
        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetWndHnd@4")]
        public extern static unsafe void SetWndHandler(IntPtr wndHandler);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetWndParams@8")]
        public extern static unsafe void SetWndParameters(int width, int height);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetKeyPressed@4")]
        public extern static unsafe void SetKeyPressed(int keyId);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetKeyReleased@4")]
        public extern static unsafe void SetKeyReleased(int keyId);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetMousePosition@8")]
        public extern static unsafe void SetMousePosition(int x, int y);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_UpdateViewport@8")]
        public extern static unsafe void UpdateViewport(int width, int height);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "_SetMouseAction@16")]
        public extern static unsafe void SetMouseAction(int x, int y, int button, int state);

    }
}
