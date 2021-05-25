# see https://stackoverflow.com/questions/2074144/generate-c-sharp-project-using-cmake
# https://stackoverflow.com/questions/2074144/generate-c-sharp-project-using-cmake/55602125#55602125

#enable the C# language support.
enable_language(CSharp)

# Include CMake utilities for CSharp, for WinForm and WPF application support.
include(CSharpUtilities)

# Define the executable, including any .cs files. And (settings, properties, resx, config)
# The .resx and other Properties files are optional here, but including them makes them visible in the VS solution for easy editing.
add_executable(ffviii_demaster_manager
        DemasterOpt.cs
        DemasterOpt.Designer.cs
        DemasterOpt.resx
        Form1.cs
        Form1.Designer.cs
        Form1.resx
        Program.cs
        zzz.cs
        zzzWatcher.cs
        zzzWatcher.Designer.cs
        zzzWatcher.resx
        App.config

        Properties/AssemblyInfo.cs
        Properties/Resources.Designer.cs
        Properties/Resources.resx
        Properties/Settings.Designer.cs
        Properties/Settings.settings
        )
csharp_set_windows_forms_properties(
        Form1.cs
        Form1.Designer.cs
        Form1.resx
        Program.cs
        Properties/AssemblyInfo.cs
        Properties/Resources.Designer.cs
        Properties/Resources.resx
        Properties/Settings.Designer.cs
        Properties/Settings.settings
)

csharp_set_windows_forms_properties(
        zzzWatcher.cs
        zzzWatcher.Designer.cs
        zzzWatcher.resx
        Program.cs
        Properties/AssemblyInfo.cs
        Properties/Resources.Designer.cs
        Properties/Resources.resx
        Properties/Settings.Designer.cs
        Properties/Settings.settings
)
csharp_set_windows_forms_properties(
        DemasterOpt.cs
        DemasterOpt.Designer.cs
        DemasterOpt.resx
        Program.cs
        Properties/AssemblyInfo.cs
        Properties/Resources.Designer.cs
        Properties/Resources.resx
        Properties/Settings.Designer.cs
        Properties/Settings.settings
)

# Add in the .NET reference libraries.
set_property(TARGET ffviii_demaster_manager PROPERTY VS_DOTNET_REFERENCES
        "Microsoft.CSharp"
        "System"
        "System.Core"
        "System.Data"
        "System.Data.DataSetExtensions"
        "System.Deployment"
        "System.Drawing"
        "System.Net.Http"
        "System.Windows.Forms"
        "System.Xaml"
        "System.Xml"
        "System.Xml.Linq"
        )
# Set the application file name. #broken? It is not working.
#set_property(TARGET ffviii_demaster_manager PROPERTY OUTPUT_NAME "ffviii_demaster_manager.exe")
# Set the .NET Framework version for the executable.
set_property(TARGET ffviii_demaster_manager PROPERTY DOTNET_TARGET_FRAMEWORK_VERSION "v4.5")
# Set the C# language version (defaults to 3.0).
set(CMAKE_CSharp_FLAGS "/langversion:latest")
# Set the executable to be 32-bit.
set_property(TARGET ffviii_demaster_manager PROPERTY WIN32_EXECUTABLE TRUE)
# Set default namespace.
set_property(TARGET ffviii_demaster_manager PROPERTY VS_GLOBAL_ROOTNAMESPACE "viiidem_customlauncher")
