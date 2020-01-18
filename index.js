const { exec } = require("child_process");
const fs = require('fs');
var sha1 = require('sha1');

const verifySig = (str) => {
    const words = str.split('|');
    const bin = Buffer.from(words[0], 'base64');
    fs.writeFileSync('sha1.signature', bin);
    const ip_sha = sha1(words[1]);
    var exec_str = "./verify pbc-0.5.14/param/a1.param " + ip_sha + " sha1.signature artifacts";
    for (let i = 2; i < words.length; i++) {
        exec_str = exec_str + " " + words[i];
    }
    console.log(exec_str);
    exec(exec_str, (error, stdout, stderr) => {
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
}

module.exports = verifySig;
