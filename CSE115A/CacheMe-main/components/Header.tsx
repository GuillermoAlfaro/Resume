// This file is our logo.
import React from 'react'
import { View, StyleSheet, Image } from 'react-native'

const Header = () => {
   return (
      <View style={styles.container}>
        <Image source={require("../assets/images/CacheMeLogo.png")} style={styles.image}/>
      </View>
    )
}

const styles = StyleSheet.create({
    container: {
        alignItems: 'center',
        backgroundColor: "#EEF2FF",
        margin: 5,
    },
    image: {
        width: 230,
        height: 40,
        resizeMode: "contain",
    },
})

export default Header

