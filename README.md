

Requirements and Other Thoughts

Make sure to read this entire section before beginning your project!

1. You should back up your code to a flash drive or an online repository like dropbox, github, or Google Drive frequently (e.g., after creating a new function successfully). If you come to us and complain that your computer crashed and you lost all of your work, we’ll ask you where your backups are.

2. In Visual C++, make sure to change your project from UNICODE to Multi Byte Character set, by going to ProjectàPropertiesàConfiguration Propertiesà GeneralàCharacter Set

3. No matter what you do and how much you finish, make sure your project builds and at least runs (even if it crashes after a while). WHATEVER YOU DO, don’t turn in code that doesn’t build.

4. Whatever you do, DO NOT add any new public member functions or public data members to the DiskMultiMap or IntelWeb classes!

5. The entire project can be completed in roughly 800 lines of C++ code beyond what we've already written for you, so if your program is getting much larger than this, talk to a TA – you’re probably doing something wrong.

6. Be sure to make copious use of the C++ STL – it can make things much easier for you!

7. If you need to define your own comparison operators for our InteractionTuple struct, feel free to do so! However you MUST place this function within IntelWeb.h as an inline function or within IntelWeb.cpp.

8. If you don’t think you can finish the whole project in time, try to build your hash table without the ability to erase items – if you do so, its on-disk data structure will be much simpler since it won’t have to keep a list of available nodes. Then leave the DiskMultiMap::erase() and IntelWeb::purge() methods for last. MAKE SURE TO BACK UP YOUR WORKING CODE PRIOR TO IMPLEMENTING THESE METHODS, AS THEY’LL RESULT IN LARGE CHANGES THAT MAY CAUSE LARGE BUGS.

9. Before you write a line of code for a class, think through what data structures and algorithms you’ll need to solve the problem. For example, what file layout will you use for your disk-based open hash table? Do you need to have any additional data stored inside it to properly deal with erased nodes? How will you decide where new nodes go? Plan before you program!

10. Don’t make your program overly complex – use the simplest data structures possible that meet the requirements.

11. You MUST NOT modify any of the code in the files we provide you that you will not turn in; since you're not turning them in, we will not see those changes. We will incorporate the required files that you turn in into a project with special test versions of the other files.

12. Make sure to implement and test each class independently of the others that depend on it. Once you get the simplest class coded, get it to compile and test it with a number of different unit tests. Only once you have your first class working should you advance to the next class.

13. Try your best to meet our big-O requirements for each method in this spec. If you can’t figure out how, then solve the problem in a simpler, less efficient way, and move on. Then come back and improve the efficiency of your implementation later if you have time.

14. BACK UP FREQUENTLY! We will not accept any excuses if you lose or delete your files.

You can still get a good amount of partial credit if you implement most of the project. Why? Because if you fail to complete a class (e.g., DiskMultiMap), we will provide a correct version of that class and test it with the rest of your program. If you implemented the rest of the program properly, it should work perfectly with our version of the class you couldn’t get working, and we can give you credit for those parts of the project you completed.
But whatever you do, make sure that ALL CODE THAT YOU TURN IN BUILDS without errors under both Visual Studio and either clang++ or g++!
