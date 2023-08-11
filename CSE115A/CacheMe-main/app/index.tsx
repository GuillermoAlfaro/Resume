// Inital screen, you can sign in or be redirected to signup.
import { Link, useRouter } from 'expo-router';
import { useEffect, useState } from 'react'
import { KeyboardAvoidingView, StyleSheet, Text, TextInput, TouchableOpacity, View } from 'react-native'
import { auth, signInWithEmailAndPassword } from '../firebase'

export default function Home() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');

    const router = useRouter();
    useEffect(() => {
      const unsubscribe = auth.onAuthStateChanged(user => {
        if (user) {
          router.push("/feed");
        }
      })
  
      return unsubscribe
    }, [])

    const handleLogIn = () => {
      signInWithEmailAndPassword(auth, email, password)
      .catch((error) => {
        const errorCode = error.code;
        if(errorCode == 'auth/invalid-email'){
          alert("Please enter a valid email address");
        }else {
          alert("Credentials do not match");
        }
      });
  }




    return(
        <KeyboardAvoidingView
      style={styles.container}
      behavior="padding"
    >
      <Text style={styles.titleText}>Log in</Text>
      <View style={styles.inputContainer}>
        <TextInput
          placeholder="Email"
          placeholderTextColor={"#575757"}
          value={email}
          onChangeText={text => setEmail(text)}
          style={styles.input}
        />
        <TextInput
          placeholder="Password"
          placeholderTextColor={"#575757"}
          value={password}
          onChangeText={text => setPassword(text)}
          style={styles.input}
          secureTextEntry
        />
      </View>

      <View style={styles.buttonContainer}>
        <TouchableOpacity
          onPress={() => {handleLogIn()}}
          style={styles.button}
        >
          <Text style={styles.buttonText}>Login</Text>
        </TouchableOpacity>     
      </View>
      <Text >Don't have an account? <Link href="signup" style={styles.link}>Sign up</Link></Text>
    </KeyboardAvoidingView>
    )
}



const styles = StyleSheet.create({
    container: {
      flex: 1,
      justifyContent: 'center',
      alignItems: 'center',
      backgroundColor: '#EEF2FF'
    },
    inputContainer: {
      width: '80%',
    },
    input: {
      backgroundColor: 'white',
      paddingHorizontal: 15,
      paddingVertical: 10,
      borderRadius: 10,
      marginTop: 5, 
    },
    buttonContainer: {
      width: '80%',
      justifyContent: 'center',
      alignItems: 'center',
      marginTop: 5,
    },
    button: {
      backgroundColor: '#0782F9',
      width: '100%',
      padding: 15,
      borderRadius: 10,
      alignItems: 'center',
      marginBottom: 10
    },
    buttonOutline: {
      backgroundColor: 'white',
      marginTop: 5,
      borderColor: '#0782F9',
      borderWidth: 2,
    },
    buttonText: {
      color: 'white',
      fontWeight: '700',
      fontSize: 16,
    },
    link: {
      color: 'blue',
      fontWeight: '700',
      fontSize: 16,
      textDecorationLine: "underline",
    },
    buttonOutlineText: {
      color: '#0782F9',
      fontWeight: '700',
      fontSize: 16,
    },
    titleText: {
      color: 'black',
      fontWeight: '700',
      fontSize: 32,
      marginBottom: 5
    }
  })