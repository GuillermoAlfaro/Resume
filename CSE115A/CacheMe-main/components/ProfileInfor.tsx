import React from 'react'
import { View, Text, Image, StyleSheet } from 'react-native'
import RegularText from './Texts/regularText'


const ProfileInfo = () => {
   return (
      <View style={styles.container}>
        <Image source = {require("../assets/images/takumi.jpeg")} style={styles.profileImage}/>
        <View style={styles.profileContainer}>
            <RegularText style={styles.name} >merilynk02</RegularText>
        </View>
      </View>
    )
}

const styles = StyleSheet.create({
    container: {
        flexDirection: 'row',
        alignItems: 'center',
        margin: 10,
    },
    profileContainer: {
        marginLeft: 8,
    },
    profileImage: {
        width: 50,
        height: 50,
        borderRadius: 25,
    },
    name: {
        fontWeight: "700",
        fontSize: 16,
    }
})

export default ProfileInfo