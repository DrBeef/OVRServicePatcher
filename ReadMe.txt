Instructions for use:

* Extract the executable OVRServicePatcher_SDK0.4.exe to the desktop
* Copy the OVRService executable you wish to patch to the desktop
* Drag and drop the OVRService_x??.exe onto the OVRServicePatcher_SDK0.4.exe 
* It should then perform the patching, creating a new patched file called: OVRService_x??.exe.riftup
* Open Task Manager and stop the following processes:
** Microsoft ® Windows Based Script Host (this is the script that restarts the service if it stops, so you'll have to stop this one first)
** OVRService_x64.exe or OVRService_x86.exe if your OS is 32-bit
* Rename the OVRService executable in the C:\Program Files (x86)\Oculus\Service folder to an appropriate backup
* Copy in the newly created patched file and remove the .riftup extension
* Right click and select Run as Administrator
* If you start the Oculus World Demo, it should now be oriented correctly
* Next time you restart your machine, the service should start in the normal way, no need to manually run anything

Important notes:

Only works with SDK 0.4 OVRService
Don't try to patch the file in-situ as it won't be able to write to that folder

DISCLAIMER
==========

I take no responsibility for any damage you do to your machine or anything else in the process of using this patcher. It;s pretty trivial so very unlikely to cause any problems, but I still waive any responsibility if you choose to use this tool.