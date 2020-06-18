export class Task {
    constructor(id, name, status, filepath) {
        this.id = id;
        this.name = name;
        this.status = status;
        this.filepath = filepath;
    }

    getId() {
        return this.id;
    }
}
