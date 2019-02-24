#NsPc updates log
Author:Xin Pan
The program passed on Windows and edit by vs2015.
***
1. Add reading a .pcm file and save as a .wav file.  
  Date:2018-07-29
2. Modify the current file reading and saving mechanism. Now you can deliver a file folder as the input and the program will read all the files in the dir and save them at dest dir.  
  Date:2018-07-30
3. Add cmd params input model. Support cmd params input model. User can specify the input filefolder and write filefolder. e.g. wav files save at `E:\test\` and the first param will be `E:\test\` ; the second param is the save path, for example the true save path is `E:\test-out` and the input param will be `E:\test-out\`  