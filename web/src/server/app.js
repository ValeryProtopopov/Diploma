import Express from 'express';
import { resolve } from 'path';

const __dirname = resolve();
const app = Express();

app.use(Express.static(resolve(__dirname, '/public')));
app.use(Express.static(resolve(__dirname, '/dist/bundle.js')));
app.use(Express.static(resolve(__dirname, '/build/client')));
app.use(Express.static(__dirname + '/src/client'));
app.use(Express.static(__dirname + '/dist'));
app.use(Express.static(__dirname + '/public'));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/public/index.html');
});

export default app;