In the case of recurring crashes in one location or related to a certain effect or graphic like the map screen you may be able to fix the issues by getting rid of Nvidia's shader caching. 
We won't benefit much from having a cache anyway if you use mods that change the shaders frequently, though you may experience stutter loading the environment. 
I have no issues or decreased performance after doing this but I am using a powerful PC so ymmmv

If you'd like to first confirm this is the issue you can globally disable shader caching in Nvidia control panel and then either restart your PC or close any Nvidia processes, 
press windows+R or prepare to type in a file explorer address bar, and wipe these two locations

%LOCALAPPDATA%\NVIDIA\GLCache
%LOCALAPPDATA%\NVIDIA\DXCache

This actually fixed a hang issue for me immediately and I was able to reenable my cache, however the issue did eventually come back since I enabled caching again. 
You can try running DDU in safe mode and updating drivers or wipe the other cache locations manually:

%PROGRAMDATA%\NVIDIA Corporation\NV_Cache
%APPDATA%\NVIDIA\ComputeCache​
%LOCALAPPDATA%\D3DSCache
%APPDATA%\..\LocalLow\NVIDIA\PerDriverVersion\DXCache
%APPDATA%\..\LocalLow\NVIDIA\PerDriverVersion\GLCache

But if there's a buggy shader its going to keep happening. I think my repeated testing and compilation of hundreds of reshade shaders was the main issue here. 
If you want to preempt any issues or you've already confirmed caching is a problem for you, don't just globally disable your cache. 
You definitely want 10GB or more for other games and applications. 

Instead download [Nvidia Profile Inspector](https://github.com/Orbmu2k/nvidiaProfileInspector/releases) find Genshin Impact, scroll down to section 5 - Common
There you will see Shadercache - Cachesize and Shadercache-Enabled which can both be set to off to override global caching for Genshin only. 
In this section you can also enable rebar for a possible performance gain. You can also set any other tweaks that are exposed in nvidia control panel such as vsync, ultra low latency
or forced anisotropic filtering. I don't recommend messing with any of the other settings in here if you don't know exactly what you're doing. I do plan to see if anything in here is useful anyway

You may need to restart PC after applying changes. Do so and see if the issues are fixed. Most likely they will be but in the case of continue problems please raise a new issue 

