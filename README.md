# FindProfileOwner
Given a metadata decrypted Android userdata partition, you can identify what inode has the profile owner file.
Google's parental controls are used as an example in this repo. You might have to change the expected size to the appropriate size of your profile owner.

This is not a practical tool at ALL. There is no guarantee the detected inode 100% has the profile owner file. I don't even know how to metadata decrypt a non emulator userdata image.
That being said, I hope that you find this simple script useful for whatever strange project you're working on :D

To compile, first install dependencies: 

`sudo apt-get install uuid-dev e2fslibs-dev`

Then you can compile

`g++ main.cpp -o main -lext2fs -luuid -lcom_err`
