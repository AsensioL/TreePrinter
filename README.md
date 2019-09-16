# TreePrinter
C++ Console Printer for Trees with Any Number of Children

## History
After searching a little bit on the Internet and not finding anything that would do the job I decided to do it my self (and later decided to share it with the community).

I hope there isn't any flagrant errors on my code (I am Aerospace major, not CS, altough I took some CS classes).

## Idea
To create a superclass that embraces the head node of a tree (where nodes can have an arbitrary number of children).

By doing so, it enables to print the whole tree on the console without messing the original node class.

It uses a templated class and a set-up function to avoid the hustle of diving into the internals.

## How to use it
### Step 1
Copy the BTNode Class, from the `src` folder to your project folder.

### Step 2
Your node class needs 2 things. You most likely have them already, or it's trivial to add them if you are building this kind of tree.
1. A getChildren member function of your node class that
    1. takes no parameters, and
    2. returns a constant reference to a standard list of pointer to its children (aka `const std::list<YourClass *> & YourClass::getChildrenFunction()`)
2. A getData member function of your node class that
    1. takes no parameters, and
    2. returns a constant reference to a standard string of its content (aka `const std::string & YourClass::getStringFunction()`)

### Step 3
Set up the templated class in *your code* before using it:
```cpp
BTNode<YourClass>::initializeClass(&YourClass::getChildrenFunction, &YourClass::getStringFunction);
```

### Step 4
Initialize the BTNode with a pointer to the main node, and ask it to print the tree:
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
Example 2
```text
      _______________me_______________
     /           /      \    \        \
   _Paul_       Anna  Sarah Emma    _Tina
  /      \       |                 /     \
Kevin Elloine Anthony            Susan Jason
```
Example 3
```text
                     _________________________________me_________________________________
                    /                                   \          \        \            \
   ________________Paul_______________               Anthony     Sarah     Emma        _Tina
  /      /                            \                 |                   |         /     \
Kevin Antoine                 _____Elloine_____     Mary Jane             _Eva__    Susan Jason
                             /                 \    /       \            /      \
                  _________Hector_________    Tom Edison Chaplin       Oliver Harry
                 /       /        \       \
              Charlie Jennifer Margaret Megan
```
