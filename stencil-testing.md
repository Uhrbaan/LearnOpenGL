# Stencil testing
the stencil buffer is used to discard specific fragments.

* ebable writing to stencil buffer

        glEnable(GL_STENCIL_TEST);
* render, update stencil buffer
* render, discard according to stencil buffer

        // clear buffer each frame
        glClear(GL_STENCIL_BUFFER_BIT);

with `glStencilMask()` you can set a bit mask that is `AND`ed to the stencil buffer
```c
glStencilMask(0xFF); // each bit is written to as is
glStencilMask(0x00); // each bit ends up as 0 (disabling writes)
```

## buffer behaviour
you can customize the behaviour of the stencil buffer through `glStencilFunc` to determine which values get discarded.

         glStencilFunc(GLenum func, GLint ref, GLuint mask)

* **func**: determines test function which decides if 
            passes of is discarded. Possible otions are:`
            GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL
            GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS`
* **ref**:  value the function is compared to
* **mask**: specifies a mask that is `AND`ed with
            reference value and stencil buffer.

        glStencilFunc(GL_EQUAL, 1, 0xFF); // example

## buffer mask
to update the buffer whith which it is tested, you use `glStencilOp`.

        glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
* **sfail**: action if *stencil* test fails.
* **dpfail**: action if *stencil* passes but *depth* passes.
* **dppass**: action if both pass.

| action  | descpription |
|:-------:|:------------:|
| `GL_KEEP`     | The currently stored stencil value is kept. |
| `GL_ZERO`     | The stencil value is set to 0. |
| `GL_REPLACE`  | The stencil value is replaced with the reference value set with `glStencilFunc`. |
| `GL_INCR`     | The stencil value is increased by 1 if it is lower than the maximum value. |
| `GL_INCR_WRAP`| Same as `GL_INCR`, but wraps it back to 0 as soon as the maximum value is exceeded. |
| `GL_DECR`     | The stencil value is decreased by 1 if it is higher than the minimum value. |
| `GL_DECR_WRAP`| Same as `GL_DECR`, but wraps it to the maximum value if it ends up lower than 0. |
| `GL_INVERT`   | Bitwise inverts the current stencil buffer value. |

by default, `glStencilOp` is set to `(GL_KEEP, GL_KEEP, GL_KEEP)`.

### example: object outline
1. Enable stencil writing.
2. Set the stencil op to `GL_ALWAYS` before drawing the (to be
   outlined) objects, updating the
   stencil buffer with 1s wherever the objects’ fragments are
   rendered.
3. Render the objects.
4. Disable stencil writing and depth testing.
5. Scale each of the objects by a small amount.
6. Use a different fragment shader that outputs a single (border)
   color.
7. Draw the objects again, but only if their fragments’ stencil
   values are not equal to 1.
8. Enable depth testing again and restore stencil func to GL_KEEP.

#### solution

first a fragment shader for outline color
~~~glsl
void main()
{
FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
~~~

then enable stencil testing

        glEnable(GL_STENCIL_TEST);

> If any of the tests fail we do nothing; we simply keep the currently stored value that is in the
stencil buffer. If both the stencil test and the depth test succeed however, we want to replace the
stored stencil value with the reference value set via `glStencilFunc` which we later set to `1`.

~~~C
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments pass the stencil test
glStencilMask(0xFF); // enable writing to the stencil buffer
normalShader.use();
DrawTwoContainers();
~~~

