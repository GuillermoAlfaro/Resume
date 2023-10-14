class Cube {
    constructor() {
        this.type = "cube";
        this.color = [1.0, 1.0, 1.0, 1.0];

        this.matrix = new Matrix4();
        this.normalMatrix = new Matrix4();
        this.textureNum = -2;
        this.cubeVerts = [
            0, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 1, 0, 0, 1, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 0, 1, 1, 0, 0,
            0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 1, 1,
            0, 0, 0, 0, 1, 1, 0, 1, 0,
            1, 0, 0, 1, 0, 1, 1, 1, 1,
            1, 0, 0, 1, 1, 1, 1, 1, 0,
            1, 0, 1, 0, 0, 1, 0, 1, 1,
            1, 0, 1, 0, 1, 1, 1, 1, 1
        ];
    }

    render() {
        var rgba = this.color;

        gl.uniform1i(u_whichTexture, this.textureNum);
        gl.uniformMatrix4fv(u_ModalMatrix, false, this.matrix.elements);

        //front//
        gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3]);
        drawTriangle3DUVNormal([0, 0, 0, 1, 1, 0, 1, 0, 0], [0, 0, 1, 1, 1, 0], [0, 0, -1, 0, 0, -1, 0, 0, -1]);
        drawTriangle3DUVNormal([0, 0, 0, 0, 1, 0, 1, 1, 0], [0, 0, 0, 1, 1, 1], [0, 0, -1, 0, 0, -1, 0, 0, -1]);

        //top//
        //gl.uniform4f(u_FragColor, rgba[0] * .9, rgba[1] * .9, rgba[2] * .9, rgba[3] * .9);
        drawTriangle3DUVNormal([0, 1, 0, 0, 1, 1, 1, 1, 1], [0, 0, 0, 1, 1, 1], [0, 1, 0, 0, 1, 0, 0, 1, 0]);
        drawTriangle3DUVNormal([0, 1, 0, 1, 1, 1, 1, 1, 0], [0, 0, 1, 1, 1, 0], [0, 1, 0, 0, 1, 0, 0, 1, 0]);

        //bottom//
        //gl.uniform4f(u_FragColor, rgba[0] * .6, rgba[1] * .6, rgba[2] * .6, rgba[3] * .6);
        drawTriangle3DUVNormal([0, 0, 1, 1, 0, 1, 1, 0, 0], [0, 0, 0, 1, 1, 1], [0, -1, 0, 0, -1, 0, 0, -1, 0]);
        drawTriangle3DUVNormal([0, 0, 1, 1, 0, 0, 0, 0, 0], [0, 0, 1, 1, 1, 0], [0, -1, 0, 0, -1, 0, 0, -1, 0]);

        // //left//
        //gl.uniform4f(u_FragColor, rgba[0] * .4, rgba[1] * .4, rgba[2] * .4, rgba[3] * .4);
        drawTriangle3DUVNormal([0, 0, 0, 0, 0, 1, 0, 1, 1], [0, 0, 0, 1, 1, 1], [-1, 0, 0, -1, 0, 0, -1, 0, 0]);
        drawTriangle3DUVNormal([0, 0, 0, 0, 1, 1, 0, 1, 0], [0, 0, 1, 1, 1, 0], [-1, 0, 0, -1, 0, 0, -1, 0, 0]);

        // //right//
        //gl.uniform4f(u_FragColor, rgba[0] - .25, rgba[1] - .25, rgba[2] - .25, rgba[3]);
        drawTriangle3DUVNormal([1, 0, 0, 1, 0, 1, 1, 1, 1], [1, 1, 1, 0, 0, 0], [1, 0, 0, 1, 0, 0, 1, 0, 0]);
        drawTriangle3DUVNormal([1, 0, 0, 1, 1, 1, 1, 1, 0], [1, 1, 0, 0, 0, 1], [1, 0, 0, 1, 0, 0, 1, 0, 0]);

        // //back//
        //gl.uniform4f(u_FragColor, rgba[0] * .6, rgba[1] * .6, rgba[2] * .6, rgba[3] * .6);
        drawTriangle3DUVNormal([1, 0, 1, 0, 0, 1, 0, 1, 1], [0, 0, 0, 1, 1, 1], [0, 0, 1, 0, 0, 1, 0, 0, 1]);
        drawTriangle3DUVNormal([1, 0, 1, 0, 1, 1, 1, 1, 1], [0, 0, 1, 1, 1, 0], [0, 0, 1, 0, 0, 1, 0, 0, 1]);

    }

    renderfaster() {
        var rgba = this.color

        gl.uniform1i(u_whichTexture, -2);

        gl.uniform4f(u_FragColor, rgba[0], rgba[1], rgba[2], rgba[3])

        gl.uniformMatrix4fv(u_ModalMatrix, false, this.matrix.elements)

        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.cubeVerts), gl.DYNAMIC_DRAW)

        gl.drawArrays(gl.TRIANGLES, 0, 36);
    }
}
