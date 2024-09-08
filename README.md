# Start X Back

This repository is deprecated and no further support will be provided. I recommend someone reverse-engineers this method and writes their own tool.

Additionally, please support the developer of this software if you can.

A universal crack for StartIsBack and StartAllBack which should support all versions.

Note:
The about page will report "SIGNATURE CHECK FAILED" for StartIsBack, ignore it - it does not affect activation.

However, if it irks you, generate a self signed codesigning certificate and add it to your Trusted Root CA. Then sign the patched DLLs, and then it will report "Your license is activated".

Alternatively, place version.dll from releases into the same directory as StartIsBackCfg.exe and then it will report "Your license is activated".

Source code for version.dll is provided under StartIsGenuine.