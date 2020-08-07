import fs from 'fs';

export class Task {
    constructor(id, name, status, dirpath) {
        this.id = id;
        this.name = name;
        this.status = status;
        this.dirpath = dirpath;
    }

    getId() {
        return this.id;
    }

    getName() {
        return this.name;
    }

    getStatus() {
        return this.status;
    }

    getDirpath() {
        return this.dirpath;
    }

    getMatrix() {
        fs.readFile(this.dirpath + 'matrix.txt', 'utf8', (err, contents) => {
            if (err) {
                throw err;
            }
            return contents;
        })
    }

    getVector() {
        fs.readFile(this.dirpath + 'vector.txt', 'utf8', (err, contents) => {
            if (err) {
                throw err;
            }
            return contents;
        })
    }
}
