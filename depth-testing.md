# Depth testing
 * enable depth texting
 
       glEnable(GL_DEPTH_TEST);
 * clear depth buffer:

       glClear(GL_DEPTH_BUFFER_BIT);

 * disable updates to depth buffer:

        glDepthMask(GL_FALSE);
 * change depth buffer testing:

        glDepthFunc(...) p.175
 * prevent Z-fighting:
    * dont place objects which overlap
 *  place the near-plane further back (causes clipping if too much)
    * works because depth precision is greater the nearer the near-plane
 * increase to 32bit depth buffer (at performance cost)