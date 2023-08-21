import SwiftUI
struct ContentView: View {
  
    var body: some View {
        TabView {
            Home()
                .tabItem{
                    Text("Home")
                        .foregroundColor(.white)
                        .bold()
                }
            Photos()
                .tabItem {
                    Text("Photos")
                        .foregroundColor(.white)
                        .bold()
                }
        }
    }
}
