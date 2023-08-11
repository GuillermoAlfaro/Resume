module.exports = function (api) {
  api.cache(true);
  return {
    presets: ["babel-preset-expo"],
    plugins: [
      require.resolve("expo-router/babel"),
      ["module:react-native-dotenv", {
        "envName": "APP_ENV",
        "moduleName": "@env",
        "path": ".env"
      }],
      ["module-resolver", {
        "root": ["./src"],
        "alias": {
          "test": "./test",
          "underscore": "lodash"
        }
      }]
    ],
    
  };
};
