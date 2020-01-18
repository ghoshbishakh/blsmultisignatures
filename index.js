const { exec } = require("child_process");

exec("./verify pbc-0.5.14/param/a1.param ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad.signature artifacts 1 3 4", (error, stdout, stderr) => {
    if (error) {
        console.log(`error: ${error.message}`);
        return;
    }
    if (stderr) {
        console.log(`stderr: ${stderr}`);
        return;
    }
    console.log(`stdout: ${stdout}`);
});