
This is an all in one installation tool and launcher for the major safe Genshin mods, fps_unlocker, 3dmigoto, and ReShade. The only things you need prepared are the game itself, an existing 3dmigoto folder with any mods and shaderfixes you desire, and any reshade shaders outside the core library you wish to use.
This project is still very early but its feature complete and should work for most people. One of the major goals of the project is to provide a safe and free option to handle this as the only alternatives up until now have been DIY or paying a scammer monthly fees for work they didn't produce
I believe in its current state this project already far surpasses any paid solution and I intend to add more features to improve QOL and fulfill reasonable requests from users

Install should be completely brainless. If it gives you any issues at all please let me know as that's part of what I'm testing first
Just make sure to run as admin and don't try to mess with temp directories

Reshade installation is configured through command line arguments to skip the app installation phase which is normally mandatory. If an addon fails to install or you click back it will take you to that page and you will need to close both the reshade installer and the command prompt and rerun my installer
There's not many good workarounds without building a modified version of the setup tool which I don't believe would hold the same distribution rights as many authors specifically say to use the official installer, but I'll at least add an option to relaunch the installer in a future update

3dmigoto can be symbolically linked if you already have a setup. Make sure to choose a folder called 3dmigoto containing a mods and shaderfixes directory. If you don't have 3dmigoto the program will download it for you but no mods are included. You can find out more about modding by visiting the discord community (discord.gg/agmg) or visiting the genshin page on gamebanana. Please do be mindful to filter any content you don't wish to see and keep in mind that user generated mods do not necessarily reflect the 

Run genshin from "Open Genshin.exe". If this works and you have no issues then this is now your primary way to launch the game
There is only one key issue I'm tracking which is an occasional application crash/hang that has been present since updating to the newest reshade version. 
If not for that I would have had this ready over a week ago but its given me time to sort some things out. If you encounter a crash or hang please use task manager to kill your game, let me know on here or discord, and don't get baited into thinking it will come back just because you here game audio

Regardng safety, I don't belive this is any more dangerous than using any of these mods individually, which is to say not dangerous at all. None of these mods can be used for cheating, they are purely visual and I have been using them for years without issue. 
I have taken care to only utilize the injection methods used by the fps_unlock mod which MHY is aware of and condones. Much of the work here is aimed at keeping any of our mods from writing to the game folder which is the main way to trigger the anticheat.
Don't post footage including your UID or discuss this mod on official forums and I think you will be fine. Even in the case of a ban I'd expect only temporary bans if you're not using gameplay enhancing cheats
Despite that it is ultimately still a non-zero risk and users should proceed with caution. MHY could change policies at any time and do a ban wave. 
I also recommend spending some money on the game even if its just welkin. I'm personally happy to give them some money considering how relaxed their policies have been and I think its unlikely they'll ban a paying customer without evidence of cheats or attempts to ruin others' gameplay

MHY if you happen upon this repo, I assure you there's nothing new in here. This project just streamlines setup of visual mods you already turn a blind eye to. Ideally you'll let this live. In the event that you do insist on taking this down I will comply with no issue so long as the paywalled alternative genshin-impact-reshade is also dismantled and the owner is appropriately dealt with. My main goal here is to prevent users from being scammed and exposed to security risks from that author. To fix reshade I adusted its behavior to prefer loading an ini from the path where the dll is located meaning we do not read or write to the game path and only inject through the known mechanism in the fps_unlocker mod. This does enable depth buffer access but solely for the purpose of visual enhancement. Cheating is neither condoned nor tolerated by myself or anyone else involved in the works I've pulled in. Many of us are paying customers and ultimately the modding community is here because of our love for the game.


Regarding fair usage, I am okay with any of my work being modified or redistributed so long as it is not sold and source code is provided. I will add a GPL license most likely. Do be aware of any licenses from the submodules included as they may have specific terms. As an example, taking my batch installer and using it for a paywalled modloader would not be okay, but if the GIMI team wanted to include my launcher in the main repo that would be perfectly fine and is honestly something I encourage once its ready.
Regarding compensation, I am not seeking any as that would defeat the purpose of this project and may evem violate some license agreements. I can make my information available if you wish to support my other endeavors but I will never ask for anything related to this project.
Regarding contributions, there isn't a whole lot to be done other than testing but if you have some skillset that would be relevant don't hesitate to make a pull request or reach out directly. My main tasks are to improve the GUI and add more presets so any help with those would be welcome. If uploading a Reshade preset please make a pull request or message me with the file as well as a brief summary of your hardware and the general performance. I would like to have an open thread for presets in the discussions tab but I am concerned about the potential for users to upload premium shaders without rights

Some notes on crashes and fixes for advanced users

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



