export class SubTask {
    constructor(id, parent_id, name, line, vector, status) {
        this.id = id;
        this.parent_id = parent_id;
        this.name = name;
        this.line = line;
        this.vector = vector;
        this.status = status;
    }
}