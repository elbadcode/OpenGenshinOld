using Microsoft.Extensions.DependencyInjection;
using System.Diagnostics;
using System.Runtime.InteropServices;
using unlockfps_nc.Model;
using unlockfps_nc.Service;
using unlockfps_nc.Utility;

namespace unlockfps_nc
{
    internal static class Program
    {
        private static IntPtr MutexHandle = IntPtr.Zero;
        public static IServiceProvider ServiceProvider { get; private set; }

        [STAThread]
        static void Main ( )
        {
            Process existingProcess = null;
            MutexHandle = Native.CreateMutex ( IntPtr.Zero, true, @"GenshinFPSUnlocker" );
            if ( Marshal.GetLastWin32Error ( ) == 183 )
            {
                // Find the existing process
                existingProcess = Process.GetProcessesByName ( "unlockfps_nc" ).FirstOrDefault ( );
                if (existingProcess != null ) { 
                   
                    existingProcess.MainModule.Dispose ( );
                    existingProcess.CloseMainWindow ( );
                }
             }


                var services = new ServiceCollection ( );
                services.AddTransient<MainForm> ( );
                services.AddTransient<SettingsForm> ( );
                services.AddTransient<SetupForm> ( );
                services.AddSingleton<ConfigService> ( );
                services.AddSingleton<ProcessService> ( );

                ServiceProvider = services.BuildServiceProvider ( );

                ApplicationConfiguration.Initialize ( );
                Application.Run ( ServiceProvider.GetRequiredService<MainForm> ( ) );

        }
    }


    }