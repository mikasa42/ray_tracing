import numpy as np
from PIL import Image

class Ray:
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = direction / np.linalg.norm(direction)

class Sphere:
    def __init__(self, center, radius, color, specular=50):
        self.center = center
        self.radius = radius
        self.color = color
        self.specular = specular

    def intersect(self, ray):
        oc = ray.origin - self.center
        a = np.dot(ray.direction, ray.direction)
        b = 2.0 * np.dot(oc, ray.direction)
        c = np.dot(oc, oc) - self.radius * self.radius
        discriminant = b * b - 4 * a * c
        if discriminant > 0:
            t1 = (-b - np.sqrt(discriminant)) / (2.0 * a)
            t2 = (-b + np.sqrt(discriminant)) / (2.0 * a)
            if t1 > 0.001 and t2 > 0.001:
                return min(t1, t2)
        return None

    def normal(self, point):
        return (point - self.center) / np.linalg.norm(point - self.center)

class Plane:
    def __init__(self, point, normal, color, specular=10):
        self.point = point
        self.normal_vec = normal / np.linalg.norm(normal)  # Renomeei para 'normal_vec' para evitar conflitos
        self.color = color
        self.specular = specular

    def intersect(self, ray):
        denom = np.dot(self.normal_vec, ray.direction)
        if np.abs(denom) > 1e-6:
            t = np.dot(self.point - ray.origin, self.normal_vec) / denom
            if t >= 0:
                return t
        return None

    def normal(self, point):
        return self.normal_vec  # Retorna 'normal_vec' em vez de 'normal'

def reflect(direction, normal):
    return direction - 2 * np.dot(direction, normal) * normal

def lighting(point, normal, view, specular, light_position, light_intensity):
    ambient = 0.1
    light_dir = light_position - point
    light_dir = light_dir / np.linalg.norm(light_dir)
    
    diff = max(np.dot(light_dir, normal), 0)
    diffuse = diff * light_intensity
    
    reflect_dir = reflect(-light_dir, normal)
    spec = max(np.dot(reflect_dir, view), 0)
    specular = light_intensity * (spec ** specular)
    
    return ambient + diffuse + specular

def render(scene, width, height, light_position, light_intensity):
    aspect_ratio = float(width) / height
    fov = np.pi / 3
    image = np.zeros((height, width, 3))

    for j in range(height):
        for i in range(width):
            x = (2 * (i + 0.5) / width - 1) * np.tan(fov / 2) * aspect_ratio
            y = (1 - 2 * (j + 0.5) / height) * np.tan(fov / 2)
            ray = Ray(np.array([0, 0, 0]), np.array([x, y, -1]))

            color = np.array([0, 0, 0])
            min_t = float('inf')
            hit_obj = None
            hit_point = None

            for obj in scene:
                t = obj.intersect(ray)
                if t and t < min_t:
                    min_t = t
                    hit_obj = obj
                    hit_point = ray.origin + ray.direction * t

            if hit_obj:
                normal = hit_obj.normal(hit_point)
                view = -ray.direction
                intensity = lighting(hit_point, normal, view, hit_obj.specular, light_position, light_intensity)
                color = hit_obj.color * intensity

            image[j, i] = np.clip(color, 0, 1)
    
    image = (255 * image).astype(np.uint8)
    return Image.fromarray(image)

# Define a cena com duas esferas e um plano
scene = [
    Sphere(np.array([-0.5, -0.5, -3]), 0.5, np.array([1, 0, 1]), specular=100),
    Sphere(np.array([1.5, -0.5, -6]), 0.4, np.array([0.86, 0.08, 0.24]), specular=100),
    Plane(np.array([0, -1, 0]), np.array([0, 1, 0]), np.array([0.5, 0.5, 0.5]))
]

# Renderiza a imagem
light_position = np.array([5, 5, -10])
light_intensity = 1.5
image = render(scene, 800, 400, light_position, light_intensity)
image.show()
image.save('ray_tracing.png')
