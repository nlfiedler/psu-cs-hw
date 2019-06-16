/*
  Distributing N points on a sphere
  =================================

  Assuming the sphere is at the origin, the spherical transform
  (theta,phi) for each point m on the surface:

  'maxpoints' = number of points on the surface.
  'surface' = 0..360 the surface area of the sphere to use.
  'pitch' = the relationship between spiral pitch and point spacing.
  For equal distribution use distribPoints(maxpoints,360,1).
*/
     void distribPoints(int maxpoints,double surface,double pitch) {
	double surfprop;
	double ncon;
	double ddd;
	double y;
	double theta,phi;
	
	for(point=0; point<maxpoints; point++) { 
	   surfprop = 0.5*(1-cos((surface*DTOR)/2));
	   ncon = maxpoints/surfprop;
	   ddd = sqrt(pi*ncon)/p;
	   y = 1-2*point*surfprop;
	   theta = acos(y);
	   phi = ddd*asin(y)*180/pi);
	   
	   /* do stuff with theta and phi...... */
	   /* like rotx(theta), roty(phi)...... */
	}
     }

/*
Does that solve the argument?
-- 
-------------------------------------------------------------------------
 Robin Green, Technical Specialist
 Electronic Arts Ltd UK.                               EA has no opinions
-------------------------------------------------------------------------

OK, imagine a sphere at the origin. We're dealing with spherical
coordinates here, so the radius of the sphere isn't of any interest.
A spherical coordinate (theta,phi) will give us any point on the
surface of the sphere. Imagine a line drawn from the origin through
the point (theta,phi). By varying theta from 0..360 degrees (its only
degrees because they're easier to imagine) you produce a line that
points from straight up to straight down with all other inclinations
inbetween. The 'surface' parameter tells the algorithm how many degrees
of inclination to distribute the points over. In your case, you'll
want to use 360 (OK, it's SUPPOSED to be 180 degrees but this system
was written for a non-technical dumbo called William Latham.)

I apologise for DTOR, but I tend to use the Graphics Gems libraries
like other people drink water. It's a constant number used for
converting degrees to radians.

	#define DTOR 0.017453292
	#define RTOD 57.29577951

>
>: 'pitch' = the relationship between spiral pitch and point spacing.
>
>  "spiral pitch"?  "point spacing" is pixel spacing?

OK, Using this parameter you can make it distribute points more in a
spiral pattern. The program usually produces an evenly distributed
spiral of points, but this parameter will separate out the arms of the
spiral and shuffle up the points closer to each other. Looks pretty.


>:          /* do stuff with theta and phi...... */
>:          /* like rotx(theta), roty(phi)...... */
>
> Does this mean that by this point theta and phi are set?  Or do we
> have to fill in the blanks and complete the source?  What are rotx()
> and roty() supposed to do?  Assuming it's short for rotate, what
> exactly does it mean to rotate a coordinate?  I can think of three
> possible meanings in this context.

Go back to the picture of the line from the origin through the point
(theta,phi). What these 'rot' functions do is take a point (0,radius,0)
on the top of the sphere and, by rotating it through an angle of theta
in the x-axis, then through an angle of phi through the y-axis finally
arrives at the point in 3D space you are looking for. Yes, by this point
theta and phi are finished and it's your job to actually move a 3D
point to the spherical coordinate (theta,phi,radius).

>  It helps, but I have a strange aversion to things I don't understand. 
>Could you maybe elaborate as to what the algorithm is?  Maybe toss in a
>couple of comments explaining what "ddd" and "ncon" and "surfprop" 
>actually are?

OK, 'surfprop' is the proportion of the surface area the points are to
be distributed over. 'ncon' is the surface proportion to be covered
divided by the number of points to be fitted i.e. the surface proportion
each point can occupy. 'ddd' is something complicated like "diurnal
diameter distance" that I don't understand myself.
 
>  In earlier posts on this subject I've heard of something like the
>"inverse cosine" algorithm - is that what this is?  The 1-cos() seems
>like it would fit... 

Absolutely no idea. Sorry. :-(
Hope I helped.

-- 
-----------------------------------------------------------------------
 Robin Green, Technical Specialist		          rgreen@ea.com
 Electronic Arts Ltd UK.                             EA has no opinions
-----------------------------------------------------------------------

*/
