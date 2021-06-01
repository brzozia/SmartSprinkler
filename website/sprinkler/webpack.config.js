const path = require('path');

const HtmlWebpackPlugin = require('html-webpack-plugin');
const DynamicCdnWebpackPlugin = require('dynamic-cdn-webpack-plugin');

module.exports = {
    mode: 'development',
    entry: './src/index.js',
    output: {
      path: path.join(__dirname, '/dist'),
      filename: "index.bundle.js"
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
    // externals:{
    //     "react": "React",
    //     "react-dom": "react-dom",
    //     // "react-bootstrap":"reactbootstrap",
    //     // "bootstrap":"bootstrap",
    //     "@material-ui/core":"material-ui/core"
    // },
    plugins: [
        new DynamicCdnWebpackPlugin(),
        new HtmlWebpackPlugin()
    ]
};