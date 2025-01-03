
Display
=======

.. contents::
   :local:
   :depth: 1

The Display class is a container for a framebuffer object that is in charge of rendering the framebuffer object to the screen.

Is an strategy design pattern that comme with a set of pluggable renderers that can be added to the display object.
by default the display object renderers method is set to auto, this means that it will render the framebuffer with the method that is the most efficient for the current platform. TTY, SIXEL, ASCII.

Display
-------

.. doxygenclass:: tdl::Display
   :members:
   :protected-members:
   :undoc-members:

Strategy
--------

Here is a list of the strategies that the you can add to the Display object.

-----
ASCII
-----

The ASCII renderer is a basic renderer that can be used to render the framebuffer object to the screen in ASCII art, its the most basic renderer that the display object can use, but its also the less efficient.

.. doxygenclass:: tdl::AsciiMethode
   :members:
   :protected-members:
   :undoc-members: