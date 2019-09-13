# TreePrinter
C++ Console Printer for Trees with Any Number of Children

## History
After searching a little bit on the Internet and not finding anything that would do the job I decided to do it my self (and later decided to share it with the community).

I hope there isn't any flagrant errors on my code (I am Aerospace major, not CS, altough I took some CS classes).

## Idea
My idea was to create a superclass that would embrace the head node of a tree (one in which nodes can have an arbitrary number of children), and would have the capacity to print the whole tree on the console.

I ended up using a template (derived from the original class) to improve extendability, and let other people use it as a black box (without having to open and modify anything).

## How to use it
### Step 1
Copy the BTNode Class (from the `src` folder) into your project (I hope there are no conflicts).

### Step 2
You will need two things (you most likely have something similar to them if you are building this kind of tree):
1. A getChildren member function of your node class that
1.1 takes no parameters, and
1.2 returns a constant reference to a standard list of pointer to its children (aka `const std::list<YourClass *> & YourClass::getChildrenFunction()`)
2. A getData member function of your node class that
2.1 takes no parameters, and
2.2 returns a constant reference to a standard string of its content (aka `const std::string & YourClass::getStringFunction()`)

### Step 3
Set up the templated class in *your code* before using it:
```cpp
BTNode<YourClass>::initializeClass(YourClass::getChildrenFunction, YourClass::getStringFunction);
```

### Step 4
Initialize the BTNode with a pointer to your class, and ask it to print the tree:
```cpp
BTNode<YourClass> printer(headNodeOfYourTree);
printer.printTree();
```

## Results
Here are the output of the examples provided.
Example 1:
```text
  __+__    
 /     \   
 *   __*__ 
/ \ /  |  \
a b c  ^  d
      / \  
      e f 
```
Example 2 (I know there is a bug in spacing, I will fix it on the future)
```text
        _________________me___________________      
       /                 /      \    \        \     
   __Paul____          Anna   SarahEmma     Tina_   
  /          \           |                 /     \  
Kevin ElloineOOOOOOOO Anthony            Susan Jason
```
