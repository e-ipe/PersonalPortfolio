import SwiftUI

struct Home: View {
    let nums = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
    @State private var photoNum = 1
    
    var body: some View {
        VStack {
            Image("Soraya\(photoNum)")
                .resizable()
                .scaledToFit()
                .padding()
            
            Button(action: {
                photoNum = Int.random(in: 1...nums.count)
            }){
                Text("Click here")
                    .foregroundColor(.white)
                    .bold()
            }
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(.pink)
    }
}

struct Home_Previews: PreviewProvider {
    static var previews: some View {
        Home()
    }
}

