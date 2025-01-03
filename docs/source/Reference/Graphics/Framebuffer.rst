
Framebuffer
===========

.. contents::
   :local:
   :depth: 1

The Framebuffer class is a container for a collection of attachments that define the functionality of a framebuffer object.

The FrameBuffer class is an basic decorator design pattern that comme with a set of pluggable attachments that can be added to the framebuffer object.

FrameBuffer
-----------

.. doxygenclass:: tdl::FrameBuffer
   :members:
   :protected-members:
   :undoc-members:

Feature
----------

Here is a list of the features that the you can add to the FrameBuffer object.

note:: You can add only one attachment of each type to the FrameBuffer object.

---------
Placeable
---------

The placeable attachment is a basic attachment that can be used to place the framebuffer object in the Display.

.. doxygenclass:: tdl::Placeable
   :members:
   :protected-members:
   :undoc-members:

--------------
Transformation
--------------

The transformation attachment is a basic attachment that can be used to transform the framebuffer object. That include the position, the scale, the rotation and the origin of the framebuffer object.FrameBuffer

.. doxygenclass:: tdl::Transformation
   :members:
   :protected-members:
   :undoc-members: