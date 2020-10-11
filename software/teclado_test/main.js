const electron = require('electron');
const exec = require('child_process').exec;

const { app, BrowserWindow, screen } = require('electron');

// SHELL EXECUTIONS
function execute(command, callback) {
  exec(command, (error, stdout, stderr) => { 
      callback(stdout); 
  });
};

function createWindow () {
  let display = electron.screen.getPrimaryDisplay();
  let width = display.workArea.width;
  let height = display.workArea.height;
  let win = new BrowserWindow({
    alwaysOnTop: true,
    transparent:true,
    width: 800,
    height: 300,
    x: (width - 800)/2,
    y: height + 300 + 5,
    webPreferences: {
      nodeIntegration: true
    }
  });

  // and load the index.html of the app.
  win.loadFile('index.html')
  // win.webContents.openDevTools()

  // execute('python ../interface_test/main.py',(output)=>{
  //   console.log(output);
  // });
}

app.whenReady().then(createWindow);