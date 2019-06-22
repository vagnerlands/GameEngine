using System;
using System.Runtime.InteropServices;

namespace Game_Editor
{
    class RenderEngineAPI
    {
        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetWndHnd@@YGXPAUHWND__@@@Z")]
        public extern static unsafe void SetWndHandler(IntPtr wndHandler);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetWndParams@@YGXHH@Z")]
        public extern static unsafe void SetWndParameters(int width, int height);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetKeyPressed@@YGXH@Z")]
        public extern static unsafe void SetKeyPressed(int keyId);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetKeyReleased@@YGXH@Z")]
        public extern static unsafe void SetKeyReleased(int keyId);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetMousePosition@@YGXHH@Z")]
        public extern static unsafe void SetMousePosition(int x, int y);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?UpdateViewport@@YGXHH@Z")]
        public extern static unsafe void UpdateViewport(int width, int height);

        [DllImport(@"..\Binaries\GameDLL.dll",
            CallingConvention = CallingConvention.StdCall,
            CharSet = CharSet.Ansi,
            EntryPoint = "?SetMouseAction@@YGXHHHH@Z")]
        public extern static unsafe void SetMouseAction(int x, int y, int button, int state);

    }
}
