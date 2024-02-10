More instructions to come, but there are some bare minimum things I probably need to say even for testers. I'll leave the draft of my full writeup at the bottom if interested but tldr, run installer, run open genshin, let me know if break

Install should be completely brainless. If it gives you any issues at all please let me know as that's part of what I'm testing first
Just make sure to run as admin and don't try to mess with temp directories
You can safely delete the installer after setup completes
There are several bat files included that can individually handle the steps if you need to reinstall
3Dmigoto directories are symlinked during installation if you chose so and will never be directly modified. Your mods are safe

Run genshin from "Open Genshin.exe". If this works and you have no issues then this is now your primary way to launch the game
There is only one key issue I'm tracking which is an occasional application crash/hang that has been present since updating to the newest reshade version. 
If not for that I would have had this ready over a week ago but its given me time to sort some things out. If you encounter a crash or hang please use task manager to kill your game, let me know on here or discord, and don't get baited into thinking it will come back just because you here game audio

Notes on crashes and how I believe I fixed it will follow, however don't bother with troubleshooting if you're not savvy and motivated. I will fix any and all issues for the majority of users before my full release. I'm just looking for feedback right now, I don't want to provide active support to individuals just yet, but you can experiement on your own if you feel capable.

I believe some issues I experienced were due to some PC specific problems I've been having including the fact that I have both an nvidia gpu and an amd iGPU that can't share graphics buffers. Othewise I think its just a matter of some configuration options. Initially this reshade version had some IMGUI overlay issues, which I believe are resolved. Generally the hang I have had a significant reduction in crashes since disabling caching in 3dmigoto, reshade, and genshin itself. To disable shader caching for genshin without globally disabling it in nvidia control panel I had to use nvidia profile inspector. Crashes typically would happen in the same place consistently after the game ran for some time indicating caching behaviors. It also frequently happened on the map screen. I have already configured the reshade.ini for this but do make sure you don't have any "depth copies before clear" selected in reshade as that does seem to be one of the issues. I plan to give out better presets but for now just try to configure a couple presets to your liking, leave reshade set to only load enabled effects, and then just choose between presets to change settings. This will improve general stability. 

I added an option to the fps_unlocker to use gshade instead of reshade. initially this was more reliable but since gshade updated to match reshade versions it has become less so and I would not advise using it in this release. However, you may want to run their installer or download straight from the repo as there is an excellent library of shaders and presets available with almost complete backwards compatiblity with reshade. Theres also an option to use or not use 3dmigoto and options to hook additional DLLs that are both disabled at the moment. I feel bad about treating people like babies but its better this way. If you want to load DLLs there is another build of the fps_unlocker or you can use the official release and manually add the 3dmigoto and reshade libraries but there's little to gain here. 


A note about performance:

If your PC is decent (rtx 20 series or newer) and you never used FPS unlocker before, you will gain performance even with ReShade
FPS does not scale well with high end hardware, especially when loading lots of mods. With high performance reshade presets
and mods I get 100-150 FPS typically in the overworld with drops to 80 in cities while Abyss/domains run at 300+ FPS. With ray tracing 
I can run only barely above 60 FPS. If I disable 3dmigoto mods by pressing F6 I immediately gain 40-50 FPS so that is the bottleneck.
These stats I just listed are true for my current build with a 4070 and 7900x but it was exactly the same with 3060 ti and 5800x!
This is what I mean by doesn't scale with hardware and honestly its completely fine since you get good frames when it matters. 
I just want to make this clear for people who don't have experience using fps unlocker and get disappointing results


To be expanded in the full release, a brief explanation of the motive behind this and an open letter to MHY if they happen upon this:

This project is nothing new. All I've done here is patched a few things and streamlined setup process for known safe mods, with fps_unlocker as the main entry point. Source code will be made available and linked in due time but the major noteworthy change here is unlocking reshade addon support and changing its behavior to prefer loading an ini from the path where the dll is located meaning no writing to the game directory and getting blocked by the anticheat. This does enable depth buffer access but solely for the purpose of visual enhancement. Cheating is neither condoned nor tolerated by myself or anyone else involved in the works I've pulled in. 



What is it:

This is a new launcher that combines the capabilities of fps_unlocker, 3dmigoto, and reshade into a single executable without having to launch multiple processes or worry about timing. While there are some useful tweaks to compatibility, the core functionality has always been available to those who know how and don't mind the launch process. This tool is called Open Genshin because you literally use it to open Genshin, but also because the driving ideal is openness for the mod community. More specifically this tool is intended to displace the bad actors in the scene who are actively scamming users and illegally profiting off of plagiarized or unlicensed work. Specifically one individual has maintained a tool that provides the same function as my launcher but with access to certain features paywalled in the scummiest way possible with the program actively deleting benefits (that they were not authorized to sell) when subscriptions run out which is almost unheard of for this kind of program. Their launcher also includes spyware and has many other dubious qualities, though its true nature is not obvious at first. Despite these many issues, they have somehow maintained a large user base solely due to being the only option for most people. But we can put that to rest with the release of this tool which will always remain completely free and is designed to respect author rights, especially when it comes to the redistribution of ReShade effects. Other repositories with similar patches to use ReShade include pirated ReShade effects such as the MartyMcFly rayshading effects. I am not going to actively wage a war against these people but I do hope that by providing this tool set for free I can put a stop to their unjust and unearned rewards. If you're reading this, do better as a person and a creator. Provide something new and original and then there will be no issue with you taking money. 

MHY if you happen upon this repo, I assure you there's nothing new in here beyond the visual mods you already turn a blind eye to. Ideally you'll let this live. In the event that you do insist on taking this down I will comply with no issue so long as the paywalled launcher is also dismantled and the scammer is appropriately dealt with. 

