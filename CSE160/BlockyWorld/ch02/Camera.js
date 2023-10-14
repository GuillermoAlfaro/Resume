class Camera {
    constructor() {
        this.fov = 45

        this.eye = new Vector3([5, .35, -5]);
        this.at = new Vector3([0, .35, 0]);
        this.up = new Vector3([0, 1, 0]);

        this.viewMatrix = new Matrix4()

        this.viewMatrix.setLookAt(
            this.eye.elements[0], this.eye.elements[1], this.eye.elements[2],
            this.at.elements[0], this.at.elements[1], this.at.elements[2],
            this.up.elements[0], this.up.elements[1], this.up.elements[2]
        )

        this.projectionMatrix = new Matrix4()
        this.projectionMatrix.setPerspective(this.fov, 1, 0.1, 1000)
    }

    forward() {
        let f = new Vector3();
        f.set(this.at);
        f.sub(this.eye);
        f.normalize();
        f.mul(.2);
        this.eye.add(f)
        this.at.add(f)
        this.update()
    }

    back() {
        let f = new Vector3();
        f.set(this.eye);
        f.sub(this.at);
        f.normalize();
        f.mul(.2);
        this.eye.add(f)
        this.at.add(f)
        this.update()
    }

    left() {
        let f = new Vector3();
        f.set(this.at)
        f.sub(this.eye)
        let s = new Vector3();
        s = Vector3.cross(this.up, f);
        s.normalize()
        s.mul(.2)
        this.eye.add(s);
        this.at.add(s);
        this.update()
    }

    right() {
        let f = new Vector3();
        f.set(this.at)
        f.sub(this.eye)
        let s = new Vector3();
        s = Vector3.cross(this.up, f);
        s.normalize()
        s.mul(.2)
        this.eye.sub(s);
        this.at.sub(s);
        this.update()
    }

    turnLeft(n) {
        let f = new Vector3();
        f.set(this.at);
        f.sub(this.eye);
        var rotationMatrix = new Matrix4();
        rotationMatrix.setRotate(n, this.up.elements[0], this.up.elements[1], this.up.elements[2]);
        let f_prime = rotationMatrix.multiplyVector3(f);
        let fakeEye = new Vector3();
        fakeEye.set(this.eye)
        fakeEye.add(f_prime);
        this.at.set(fakeEye);
        this.update()
    }

    turnRight(n) {
        let f = new Vector3();
        f.set(this.at);
        f.sub(this.eye);
        var rotationMatrix = new Matrix4();
        rotationMatrix.setRotate(-n, this.up.elements[0], this.up.elements[1], this.up.elements[2]);
        let f_prime = rotationMatrix.multiplyVector3(f);
        let fakeEye = new Vector3();
        fakeEye.set(this.eye)
        fakeEye.add(f_prime);
        this.at.set(fakeEye);
        this.update()
    }

    moveUp() {
        let f = new Vector3([0, .5, 0]);
        this.eye.add(f);
        this.at.add(f);
        this.update()
    }

    moveUpn(n) {
        let f = new Vector3([0, n, 0]);
        this.eye.add(f);
        this.at.add(f);
        this.update()
    }

    moveDown() {
        let f = new Vector3([0, .5, 0]);
        this.eye.sub(f);
        this.at.sub(f);
        this.update()
    }
    moveDownn(n) {
        let f = new Vector3([0, n, 0]);
        this.eye.sub(f);
        this.at.sub(f);
        this.update()
    }

    lookDown(n) {
        let f = new Vector3([0, n * .01, 0]);
        this.at.sub(f);
        this.update()
    }

    lookUp(n) {
        let f = new Vector3([0, n * .01, 0]);
        this.at.add(f);
        this.update()
    }

    update() {
        this.viewMatrix.setLookAt(
            this.eye.elements[0], this.eye.elements[1], this.eye.elements[2],
            this.at.elements[0], this.at.elements[1], this.at.elements[2],
            this.up.elements[0], this.up.elements[1], this.up.elements[2]
        )

    }
}