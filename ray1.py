import numpy as np
import math as m
from tqdm import tqdm

RNG = np.random.default_rng()

def clamp(c, mi, ma):
    return max(min(c, ma), mi)

def random_in_unit_sphere():
    while(True):
        #p will be a numpy vector
        p = 2 * RNG.random(3) - 1
        if(np.dot(p, p) >= 1):
            continue
        
        return p

class hit_record:
    
    p = np.array([0,0,0], dtype = 'float64')
    normal = np.array([0,0,0], dtype = 'float64')
    t = 0.0
    front_face = True
    
    def __init__(self, root, at, norm, o, d):
        self.t = root
        self.p = at
        self.front_face = np.dot(d, norm) < 0
        self.normal = norm if self.front_face else -norm



class sphere:
    center = np.array([], dtype = 'float64')
    radius = 0.0
    
    def __init__(self, cen, r):
        self.center = cen
        self.radius = r
 
    def hit(self, o, d, t_min, t_max): 
        oc = o - self.center
        a = np.dot(d, d)
        half_b = np.dot(oc, d)       
        c = np.dot(oc, oc) - self.radius * self.radius
        
        discri = half_b * half_b - a*c
        if(discri < 0):
            return False

        sqrtd = m.sqrt(discri)
        root = (-half_b - sqrtd) / a

        if(root < t_min or t_max < root):
            root = (-half_b + sqrtd) / a

            if(root < t_min or t_max < root):
                return False
        
        t = root
        p = o + root * d
        n = (p - self.center) / self.radius
        
        return hit_record(t, p, n, o, d)


aspect_ratio = 16.0 / 9.0
image_width = 100
image_height = (int)(image_width/aspect_ratio)
samples_per_pixel = 1
max_depth = 10

print("P3")
print(str(image_width) + " " + str(image_height))
print(255)

class camera:

    origin = np.array([0, 0, 0], dtype = 'float64')
    lower_left_corner = np.array([0, 0, 0], dtype = 'float64')
    horizontal = np.array([0, 0, 0], dtype = 'float64')
    vertical = np.array([0, 0, 0], dtype = 'float64')

    def __init__(self):
        aspect_ratio = 16.0 / 9.0
        viewport_height = 2.0
        viewport_width = aspect_ratio*viewport_height
        focal_length = 1.0

        self.origin = np.array([0,0,0], dtype = 'float64')
        self.horizontal = np.array([viewport_width,0.0,0.0], dtype = 'float64')
        self.vertical = np.array([0.0,viewport_height,0.0], dtype = 'float64')
        self.lower_left_corner = self.origin - self.horizontal/2 - self.vertical/2 - np.array([0, 0, focal_length], dtype = 'float64')
        

    def get_ray(self, u, v):
        return self.origin, self.lower_left_corner + u*self.horizontal + v*self.vertical - self.origin

def write_color(pixel_color, samples_per_pixel):
    r = pixel_color[0]
    g = pixel_color[1]
    b = pixel_color[2]

    scale = 1.0 / samples_per_pixel

    r = m.sqrt(scale * r)
    g = m.sqrt(scale * g)
    b = m.sqrt(scale * b)

    print( str(int(255.999 * clamp(r, 0.0, 0.999))) + " " + str(int(255.999 * clamp(g, 0.0, 0.999))) 
    + " " + str(int(255.999 * clamp(b, 0.0, 0.999))) )
    

def hittable_list(spheres, o, d, t_min, t_max):
    h = False
    for s in spheres:
        r = s.hit(o, d, t_min, t_max)

        if(r != False):
            h = r

    return h

def ray_c(o, d, spheres, depth):

    if(depth <= 0):
        return np.array([0, 0, 0], dtype = 'float64')

    rec = hittable_list(spheres, o, d, 0.001, np.inf)
    if(rec != False):
        target = rec.p + rec.normal + random_in_unit_sphere()
        return 0.5 * ray_c(rec.p, target - rec.p, spheres, depth - 1)

    ud = d / np.linalg.norm(d)
    t = 0.5*(ud[1] + 1.0)
    return (1.0 - t) * np.array([1.0, 1.0, 1.0], dtype = 'float64') + t*np.array([0.5, 0.7, 1.0], dtype = 'float64')

spheres = [
    sphere(np.array([0, -100.5, -1], dtype = 'float64'), 100),
    sphere(np.array([0, 0, -1], dtype = 'float64'), 0.5)
]

cam = camera()

o, d = cam.get_ray(0.00978263188, 0.605219483)

print(o, d)

h = spheres[1].hit(np.array([0, 0, 0]), np.array([0.48901854985386706, 0.27507293224334717, -1]), 0.01, np.inf)

if(h != False):
    print(h.p, h.normal, h.t, h.front_face)
else:
    print(h)


#for j in tqdm(range(image_height-1, -1, -1)):
#    for i in range(0, image_width):
#        pixel_color = np.array([0, 0, 0], dtype = 'float64')
#        
#        for s in range(0, samples_per_pixel):
#           u = (float(i) + RNG.random() ) / (image_width - 1)
#            v = (float(j) + RNG.random() ) / (image_height - 1)
#            o, d = cam.get_ray(u, v)
#            pixel_color += ray_c(o, d, spheres, max_depth) 
#        
#        write_color(pixel_color, samples_per_pixel)
