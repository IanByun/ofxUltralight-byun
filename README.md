# ofxUltralight-byun
openframeworks 0.11.0 addon for ultralight-sdk-1.0-win https://ultralig.ht/ 

Ultralight (c) 2018 Ultralight, Inc. Ultralight is a trademark of Ultralight, Inc.

JungHyun Byun (c) 2019 junghyun.byun@yonsei.ac.kr 

![image](https://user-images.githubusercontent.com/6938192/63220462-1533a380-c1c3-11e9-99ab-80f8ad70f52f.png)

#Installation/Requirements/Notes

- You  might need Windows Media Feature Pack (missing mf.dll, mfplat.dll, mfreadwrite.dll, ...). Might be found here: https://support.microsoft.com/en-us/help/3145500/media-feature-pack-list-for-windows-n-editions. But please check your Windows version, there's a lot of different versions out there and you need the correct  one to make it work.
- Addon ofxOpenCv-4.1.1: https://github.com/IanByun/ofxOpenCv-4.1.1

I need to compile for Release & x64. Compiled for debug will lead to a crash when starting the App.

For launching the examples or your own App: add the dlls found in the following folders to your Apps bin folder:
- ofxUltralight-byun\bin\x64\
- ofxOpenCv-4.1.1\bin\ (have to be unpacked first)

