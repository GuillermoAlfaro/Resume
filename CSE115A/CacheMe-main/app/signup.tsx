// Sign up page.
import { Link, useRouter } from 'expo-router';
import { useEffect, useState } from 'react'
import { KeyboardAvoidingView, StyleSheet, Text, TextInput, TouchableOpacity, View } from 'react-native'
import { auth, db, createUserWithEmailAndPassword } from '../firebase';
import { collection, setDoc, doc } from "firebase/firestore";

export default function Home() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [username, setUsername] = useState('');
    const [fn, setFn] = useState('');
    const [mi, setMi] = useState('');
    const [ln, setLn] = useState('');

    const router = useRouter();
    useEffect(() => {
      const unsubscribe = auth.onAuthStateChanged(user => {
        if (user) {
          router.push("/feed");
        }
      })
  
      return unsubscribe
    }, [])

    const handleSignUp = () => {
      const fullName = `${fn} ${mi} ${ln}`;
      createUserWithEmailAndPassword(auth, email, password)
        .then(async (result) => {
          await setDoc(doc(collection(db, "user"), auth.currentUser?.uid), { 
              __id: auth.currentUser?.uid,
              email: email,
              name: fullName,
              username: username,
              profilePicture: "default-profile-picture.jpg",
              friends: [],
              userPosts: []
            });
        })
        .catch((error) => {
          const errorCode = error.code;
          if (errorCode == 'auth/invalid-email') {
            alert("Please enter a valid email address");
          } else if (errorCode == 'auth/email-already-in-use') {
            alert("Account with that email already exists");
          } else {
            alert("Error signing up");
          }
        });
    }
  
    return(
        <KeyboardAvoidingView
      style={styles.container}
      behavior="padding"
    >
      <Text style={styles.titleText}>Create an account</Text>
      <View style={styles.inputContainer}>
        <TextInput
          placeholder="First Name"
          placeholderTextColor={"#575757"}
          value={fn}
          onChangeText={text => setFn(text)}
          style={styles.input}
        />
        <TextInput
          placeholder="Middle Initial"
          placeholderTextColor={"#575757"}
          value={mi}
          onChangeText={text => setMi(text)}
          style={[styles.input, styles.input]}
        />
        <TextInput
          placeholder="Last Name"
          placeholderTextColor={"#575757"}
          value={ln}
          onChangeText={text => setLn(text)}
          style={styles.input}
        />
        <TextInput
          placeholder="Username"
          placeholderTextColor={"#575757"}
          value={username}
          onChangeText={text => setUsername(text)}
          style={styles.input}
        />
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
          onPress={() => {handleSignUp()}}
          style={styles.button}
        >
          <Text style={styles.buttonText}>Sign up</Text>
        </TouchableOpacity>
        <Text>Already have an account? <Link href="/" style={styles.link}>Log in</Link></Text>
        
      </View>
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