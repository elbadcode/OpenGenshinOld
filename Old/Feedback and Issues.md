If you experience any problems during installation or while playing please make a new issue with the exception of erroneous Windows Defender reports or virus scans. 
These reports are false without exception and this is easily proven by checking through my source code. I will leave one pinned topic for those issues you can comment on, 
but I'll only respond to these if the number is unusually high after an update. I would advise adding this to your windows defender exclusions if it gives you any issues 
and feel free to build things yourself if you're skeptical and have time to waste. 

If you happen to see an issue with the same problem you had then you can piggyback off that. 
In the case of crashes or hangs please use task manager to kill the game, open windows event viewer and check for an application failure. 
You can include relevant info from there as well as your d3d11.log and reshade.log from the main and the main/3dm folders as well as any info about your system specs 

If you experience crashes and want to troubleshoot yourself I'd first off recommend restoring the provided ini configuration. Additionally clear any relevant caches and make sure caching is off in 3dmigoto via the d3dx.ini. 
If you experience repeated crashes or hang at a specific area or while opening your map or something like that even with mod caches cleared it may be a driver cache issue. I don't think most people will need to resort to this 
but I found some improvements in stability by disabling my Nvidia shader cache solely for Genshin which you can find instructions for [here](Troubleshooting/NvidiaCache.md)

If you have feedback that's more of a suggestion than an issue you can for now make an issue page, but I will restrict this ability if this repository becomes popular enough and instead use discussions pages. 
I'm not opening discussions right away to limit where information is distributed and to prevent the possibility of users uploading pirated shaders or screenshots of the mods in action which I prefer not to include on github.

For viewing or sharing footage and screenshots I will likely direct people towards a project page on Gamebanana or deploy a proper frontend page on here. For issues I prefer you only share text logs but links to offsite media are okay
This might seem overly strict but I'm aiming to prevent situations like someone posting media with their UID visible or with nsfw content

In the incredibly unlikely event that you receive a ban you can raise an issue or for more security contact me on discord at lobotomyx, but you must be honest if you used any additional mods and be prepared to prove you weren't also using cheats

  
