import fs from 'fs';
import readline from 'readline';
import { Task } from "./models/Task.js";
import { SubTask } from "./models/SubTask.js";

export const sumElementsOfArray = (array) => {
    let sum = 0;
    for (let i = 0; i < array.length; i++) {
        sum = sum + parseFloat(array[i]);
    }
    return sum;
};


export async function processLineByLine() {
    const fileStream = fs.createReadStream('./views/tasks/matrix.txt');

    const rl = readline.createInterface({
        input: fileStream,
        crlfDelay: Infinity
    });

    let tasks = [];
    let index = 0;
    for await (const line of rl) {
        index = index + 1;
        tasks.push(new Task(1, index, 'Скалярное произведение', 'matrix.txt'));
        console.log(`Line from file: ${line}`);
    }
    return tasks;
}

export const splitTask = (task) => {
    let matrix = task.getMatrix();
    let vector = task.getVector();
    let subTasks = [];
    let linesOfMatrix = matrix.split('\n');
    for (let i = 0; i < linesOfMatrix.length; i++){
        subTasks.push(new SubTask(i, task.getId(), 'subtask'+ i, 2, vector,0));
    }
    return subTasks;
};