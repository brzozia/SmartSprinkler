const path = require('path');

const HtmlWebpackPlugin = require('html-webpack-plugin');
const DynamicCdnWebpackPlugin = require('dynamic-cdn-webpack-plugin');
const MinifyPlugin = require("babel-minify-webpack-plugin");

module.exports = {
    mode: 'development',
    entry: './src/index.js',
    output: {
      path: path.join(__dirname, '/dist'),
      filename: "index.bundle.js"
    },
    devServer: {
        port: 3010,
        headers: {
            "Access-Control-Allow-Origin": "http://192.168.1.28"
        },

    },
    module: {
        rules: [
            {
                test: /\.(js|jsx)$/,
                exclude: /node_modules/,
                use: {
                    loader: "babel-loader"
                }
            },
            {
                test: /\.css$/,
                use:[
                    'style-loader',
                    'css-loader'
                ]
            }
        ]
    },
    externals:{
        'react': 'React',
        "react-dom": "ReactDOM",
        // "@material-ui/core/Grid": "commonjs Grid",
        // "@material-ui/core/Card": "Card",
        // "react-bootstrap":"reactbootstrap",
        // "bootstrap":"bootstrap",
        "@material-ui/core":"root MaterialUI",
    }
    // plugins: [
    //     new DynamicCdnWebpackPlugin(),
    //     new HtmlWebpackPlugin({
    //         templateContent: `
    //         <html>
    //           <body>
    //             <div id="root"></div>
    //           </body>
    //         </html>
    //       `
    //     })
    // ]
    // plugins: [
    //     new MinifyPlugin(minifyOpts, pluginOpts)
    // ]
};